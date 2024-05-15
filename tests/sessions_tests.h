#include "auth/login_manager.h"
#include "auth/session.h"
#include "auth/student.h"
#include <ctime>
#include <memory>

bool test_sessions () {
    std::shared_ptr<auth::student> s (new auth::student ("23-101393"));
    auto session = auth::session (s, time (NULL));
    session.save_session ();

    // Retrieve the same session
    auth::session session2;
    auth::session::search_sessions ("23-101393", &session2);

    return session.get_time () == session2.get_time () &&
    !session2.has_expired () && session2.is_saved ();
}

bool test_sessions_getter () {
    auto sessions = auth::login_manager::get_instance ().get_sessions ();
    return sessions[0].get_user ().get_id () == "23-101393";
}
