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
        return _session_id != -1;
    }
    // Sessions functionalities
    void save_session ();
    static bool search_sessions (std::string target_user_id, session* target_session);
    bool is_saved () const;
    bool has_expired () const;

private:
    int _session_id;
    std::shared_ptr<user> _user;
    time_t _time;
    bool _saved;

    static constexpr char file[] = "sessions.txt";
};

} // namespace auth
