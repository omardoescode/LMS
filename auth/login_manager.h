#include "auth/user.h"
#include "utils/vector.h"
#include <memory>
namespace auth {
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
    bool has_registered () const {
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
        return _current_user;
    }

    bool authenticate (auth::user&, std::string id, std::string password);

    void load_sessions ();

private:
    int _current_session_index;
    std::shared_ptr<user> _current_user;
    utils::vector<session> _sessions;


    friend class session;
};
} // namespace auth
