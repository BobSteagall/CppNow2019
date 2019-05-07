#include "transaction.hpp"

namespace tx {

transaction::atomic_tsv     transaction::sm_tsv_generator  = 0;
transaction::atomic_tx_id   transaction::sm_tx_id_generator = 0;

transaction::~transaction()
{}

transaction::transaction(int log_level, FILE* fp)
:   m_tx_id(++sm_tx_id_generator)
,   m_tx_tsv(0)
,   m_item_ptrs()
,   m_mutex()
,   m_cond()
,   m_fp(fp)
,   m_log_level(log_level)
{
    m_item_ptrs.reserve(100u);
}

void
transaction::begin()
{
    log_begin();
    m_mutex.lock();
    m_tx_tsv = ++sm_tsv_generator;
    m_mutex.unlock();
}

void
transaction::commit()
{
    log_commit();

    tx_lock lock(m_mutex);

    while (m_item_ptrs.size() != 0)
    {
        m_item_ptrs.back()->mp_owning_tx.store(nullptr);
        m_item_ptrs.pop_back();
    }
    m_cond.notify_all();
}

void
transaction::rollback()
{
    log_rollback();

    tx_lock lock(m_mutex);

    while (m_item_ptrs.size() != 0)
    {
        m_item_ptrs.back()->mp_owning_tx.store(nullptr);
        m_item_ptrs.pop_back();
    }
    m_cond.notify_all();
}

bool
transaction::acquire(lockable_item& item) noexcept
{
    while (true)
    {
        transaction*    p_curr_tx = nullptr;

        if (item.mp_owning_tx.compare_exchange_strong(p_curr_tx, this))
        {
            m_item_ptrs.push_back(&item);

            if (m_tx_tsv > item.m_last_tsv)
            {
                log_acquisition_success(item);
                item.m_last_tsv = m_tx_tsv;
                return true;
            }
            else
            {
                log_acquisition_failure(item);
                return false;
            }
        }
        else
        {
            if (p_curr_tx == this)
            {
                log_acquisition_same(item);
                return true;
            }
            else
            {
                log_acquisition_waiting(item, p_curr_tx);

                tx_lock lock(p_curr_tx->m_mutex);

                while (item.mp_owning_tx.load() == p_curr_tx)
                {
                    if (p_curr_tx->m_tx_tsv > m_tx_tsv)
                    {
                        log_acquisition_failure(item);
                        return false;
                    }
                    p_curr_tx->m_cond.wait(lock);
                }
            }
        }
    }
}

inline void
transaction::log_begin() const
{
    if (m_log_level >= 2  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "reset to TSV %zd in TX %zd\n",
                m_tx_tsv, m_tx_id);
    }
}

inline void
transaction::log_commit() const
{
    if (m_log_level >= 2  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "committing %zd records in TX %zd TSV %zd\n",
                m_item_ptrs.size(), m_tx_id, m_tx_tsv);
    }
}

inline void
transaction::log_rollback() const
{
    if (m_log_level >= 1  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "CONFLICT rolling back %zd items in TX %zd TSV %zd\n",
                m_item_ptrs.size(), m_tx_id, m_tx_tsv);
    }
}

inline void
transaction::log_acquisition_success(lockable_item const& item) const
{
    if (m_log_level >= 3  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "acquiring ITEM %zd in TX %zd TSV %zd\n",
                item.m_item_id, m_tx_id, m_tx_tsv);
    }
}

inline void
transaction::log_acquisition_failure(lockable_item const& item) const
{
    if (m_log_level >= 1  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "newer TSV %zd when acquiring ITEM %zd in TX %zd TSV %zd\n",
                item.m_last_tsv, item.m_item_id, m_tx_id, m_tx_tsv);
    }
}

inline void
transaction::log_acquisition_same(lockable_item const& item) const
{
    if (m_log_level >= 3  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "re-acquiring ITEM %zd in TX %zd TSV %zd\n",
                item.m_item_id, m_tx_id, m_tx_tsv);
    }
}

inline void
transaction::log_acquisition_waiting(lockable_item const& item, transaction* p_curr_tx) const
{
    if (m_log_level >= 3  &&  m_fp != nullptr)
    {
        fprintf(m_fp, "acquiring ITEM %zd in TX %zd TSV %zd, waiting for TX %zd TSV %zd\n",
                item.m_item_id, m_tx_id, m_tx_tsv, p_curr_tx->m_tx_id, p_curr_tx->m_tx_tsv);
    }
}

}   //- tx namespace

