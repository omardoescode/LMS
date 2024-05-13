#include "auth/session.h"
#include "auth/student.h"
#include "utils/datetime_reader.h"
#include <filesystem>
#include <memory>

bool test_sessions () {
    // Create the session
    utils::datetime_reader dtr ("now");
    std::shared_ptr<auth::student> s (new auth::student ("23-101393"));
    auth::session session (1, s, dtr.get_time ());
    session.save_session ();

    // Get the session
    auth::session session2;
    auth::session::search_sessions ("23-101393", &session2);
    if (session2.get_session_id () != 1 || session2.get_user ().get_id () != s->get_id ())
        return false;

    return true;
}
