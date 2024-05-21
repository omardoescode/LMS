#pragma once
#include "auth/user.h"
#include "utils/vector.h"
#include <memory>
namespace auth {
class session;
class session;
class login_manager {

private:
    login_manager ();
    std::unique_ptr<user> load_user (std::string id, user::Role role);

public:
    // Default operations for singletons
    // Delete the copy constructor and assignment operator
    login_manager (login_manager&)        = delete;
    void operator= (const login_manager&) = delete;

    // Get instance for singletons
    static login_manager& get_instance () {
        static login_manager instance;
        return instance;
    }

    // Helpful funcitons
    bool is_logged () const {
        return _current_session_index != -1;
    }
    // Getters
    int get_current_session_id () const {
        return _current_session_index != -1;
    }
    utils::vector<session>& get_sessions () {
        return _sessions;
    }

    std::shared_ptr<user> get_current_user () const {
        if (is_logged ())
            return _current_user;
        return nullptr;
    }


    // Authentication functions
    bool login (std::string username, std::string password);
    bool login (int session_id);
    bool logout ();

private:
    int _current_session_index;
    std::shared_ptr<user> _current_user;
    utils::vector<session> _sessions;

    void load_sessions (); // initalized by constructor
    friend class session;
};
} // namespace auth
