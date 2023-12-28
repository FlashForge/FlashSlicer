#include "ComCommandQue.hpp"
#include <utility>

namespace Slic3r { namespace GUI {

void ComCommandQue::clear()
{
    boost::mutex::scoped_lock lock(m_stateMutex);
    m_commandList.clear();
}

ComCommandPtr ComCommandQue::popFront(unsigned int milliseconds /* = -1 */)
{
    boost::mutex::scoped_lock lock(m_stateMutex);
    m_condition.wait_for(lock, boost::chrono::milliseconds(milliseconds));
    if (m_commandList.empty()) {
        return ComCommandPtr();
    } else {
        ComCommandPtr front = m_commandList.front().command;
        m_commandList.pop_front();
        return front;
    }
}

void ComCommandQue::pushBack(const ComCommandPtr &command, int priority, bool checkDup/* = false */)
{
    boost::mutex::scoped_lock lock(m_stateMutex);
    if (checkDup) {
        for (auto it = m_commandList.begin(); it != m_commandList.end(); ++it) {
            if (command->isDup(it->command.get())) {
                increasePriority(it, priority);
                return;
            }
        }
    }
    auto it = m_commandList.rbegin();
    for (; it != m_commandList.rend(); ++it) {
        if (it->priority <= priority) {
            break;
        }
    }
    QueItem item = { command, priority };
    m_commandList.insert(it.base(), item);
    m_condition.notify_one();
}

void ComCommandQue::increasePriority(const std::list<QueItem>::iterator &it, int priority)
{
    if (it->priority <= priority) {
        return;
    }
    it->priority = priority;
    auto revIt = std::make_reverse_iterator(it);
    auto tmpIt = revIt;
    for (; tmpIt != m_commandList.rend(); ++tmpIt) {
        if (tmpIt->priority <= priority) {
            break;
        }
    }
    if (tmpIt != revIt) {
        m_commandList.splice(tmpIt.base(), m_commandList, it);
    }
}

}} // namespace Slic3r::GUI
