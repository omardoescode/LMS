#include "auth/session.h"
#include "auth/student.h"
#include "utils/datetime_reader.h"
#include <ctime>
#include <memory>

bool test_sessions () {
    // Create the session
    utils::datetime_reader dtr ("now");
    std::shared_ptr<auth::student> s (new auth::student ("23-101393"));
    // std::cout << dtr.to_string ();
    auth::session session (1, s, dtr.get_time ());
    session.save_session ();


    // Get the session
    // auth::session session2;
    // auth::session::search_sessions ("23-101393", &session2);
    // std::cout << session2.get_session_id ();
    // if (dtr.get_time () != session2.get_time ())
    //     return false;

    // std::cout << (difftime (time (NULL), session2.get_time ()) > 24 * 60 * 60);


    return true;
}
