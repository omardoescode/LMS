#pragma once
#include "auth/user.h"
#include <memory>
namespace auth {
class session {
public:
    session ();
    session (std::shared_ptr<user> user, time_t time);
    // Getters
    user& get_user () {
        return *_user;
    }
    time_t get_time () const {
        return _time;
    }

    // Helpful functionalities
    bool valid_session () const {
        return _user != nullptr;
    }
    // Sessions functionalities
    void save_session ();
    bool is_saved () const;
    bool has_expired () const;
    void update_session_time_to_now ();

    // Static Sessions functionalities
    static bool search_sessions (std::string target_user_id, session* target_session);

    // Sessions directory
    static constexpr char _sessions_directory[] = "sessions";

private:
    std::shared_ptr<user> _user;
    time_t _time;

    // Helpful private functions
    static std::string generate_path (std::string username);
    void generate_directory ();
    static std::string get_directory ();

    friend class login_manager;
};


} // namespace auth
