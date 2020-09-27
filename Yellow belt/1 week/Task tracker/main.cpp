
class TeamTasks {
public:
    const TasksInfo &GetPersonTasksInfo(const std::string &person) const {
        return developers.at(person);
    }

    void AddNewTask(const std::string &person) {
        developers[person][TaskStatus::NEW]++;
    }

    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string &person, int task_count) {
        TasksInfo changed;
        TasksInfo old;

        update_task_status(changed, old, task_count, person, TaskStatus::NEW);
        update_task_status(changed, old, task_count, person, TaskStatus::IN_PROGRESS);
        update_task_status(changed, old, task_count, person, TaskStatus::TESTING);
        update_task_status(changed, old, task_count, person, TaskStatus::DONE);

        update_developers(changed, old, person);

        return {changed, old};
    }

private:
    void update_developers(const TasksInfo &changed, const TasksInfo &old, const std::string &person) {
        if (changed.count(TaskStatus::NEW) != 0) {
            developers[person][TaskStatus::NEW] = changed.at(TaskStatus::NEW);
        }

        if (changed.count(TaskStatus::IN_PROGRESS) != 0) {
            developers[person][TaskStatus::IN_PROGRESS] = changed.at(TaskStatus::IN_PROGRESS);
        }

        if (changed.count(TaskStatus::TESTING) != 0) {
            developers[person][TaskStatus::TESTING] = changed.at(TaskStatus::TESTING);
        }

        if (changed.count(TaskStatus::DONE) != 0) {
            developers[person][TaskStatus::DONE] = changed.at(TaskStatus::DONE);
        }


        if (old.count(TaskStatus::NEW) != 0) {
            developers[person][TaskStatus::NEW] += old.at(TaskStatus::NEW);
        }

        if (old.count(TaskStatus::IN_PROGRESS) != 0) {
            developers[person][TaskStatus::IN_PROGRESS] += old.at(TaskStatus::IN_PROGRESS);
        }

        if (old.count(TaskStatus::TESTING) != 0) {
            developers[person][TaskStatus::TESTING] += old.at(TaskStatus::TESTING);
        }

        if (old.count(TaskStatus::DONE) != 0) {
            developers[person][TaskStatus::DONE] += old.at(TaskStatus::DONE);
        }
    }

    void update_task_status(TasksInfo &changed, TasksInfo &old, int &task_count, const std::string &person,
                            TaskStatus status) {
        if (developers.at(person).count(status) == 0)
            return;

        if (task_count == 0) {
            if (changed.count(status) != 0)
                old[status] = developers.at(person).at(status);
            return;
        }

        if (developers.at(person).at(status) < task_count) {
            changed[static_cast<TaskStatus>(static_cast<int>(status) + 1)] = developers.at(person).at(status);
            task_count -= changed[static_cast<TaskStatus>(static_cast<int>(status) + 1)];
        } else {
            changed[static_cast<TaskStatus>(static_cast<int>(status) + 1)] = task_count;
            old[status] = developers[person][status] - task_count;
            if (old[status] == 0)
                old.erase(status);
            task_count = 0;
        }

    }

    std::map<std::string, TasksInfo> developers;
};