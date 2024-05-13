#pragma once
#include "auth/user.h"
#include <auth/login_manager.h>
#include <memory>
namespace auth {
class session {
public:
    session ();
    session (int session_id, std::shared_ptr<user> user, time_t time);
    // Getters
    int get_session_id () const {
        return _session_id;
    }
    user& get_user () {
        return *_user;
    }
    time_t get_time () const {
        return _time;
    }

    // Helpful functionalities
    bool valid_session () const {
        return _session_id > 0;
    }
    // Sessions functionalities
    void save_session ();
    bool is_saved () const;
    bool has_expired () const;
    void update_session_time_to_now ();

    // Static Sessions functionalities
    static bool search_sessions (std::string target_user_id, session* target_session);

private:
    int _session_id;
    std::shared_ptr<user> _user;
    time_t _time;

    static constexpr char sessions_directory[] = "sessions";

    // Helpful private functions
    static std::string generate_path (std::string username);
    void generate_directory ();
};

} // namespace auth
