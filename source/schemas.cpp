#include "schemas.hpp"

tm read8601(const char* dateStr) {
    int y,M,d,h,m;
    float s;
    sscanf(dateStr, "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);
    tm time = { 0 };
    time.tm_year = y - 1900; // Year since 1900
    time.tm_mon = M - 1;     // 0-11
    time.tm_mday = d;        // 1-31
    time.tm_hour = h;        // 0-23
    time.tm_min = m;         // 0-59
    time.tm_sec = (int)s;    // 0-61 (0-60 in C++11)
    return time;
}

static const char* kTypeNames[] = 
    { "Null", "False", "True", "Object", "Array", "String", "Number" };
    
int Timeline::Ingest(rapidjson::Document &doc) {
    if (!doc.IsArray()) {
        std::cout << "Timeline Ingest Error 1: Timeline (Array) is of type " << kTypeNames[doc.GetType()] << std::endl;
        return -1;
    }
    for (rapidjson::Value &v : doc.GetArray()) {
        Status newStatus = {};
        int retCode = newStatus.Ingest(v);
        if (retCode != 0)
            return retCode;
        statuses.push_back(newStatus);
    }
    return 0;
}

int Account::Ingest(rapidjson::Value &object) {
    if (!object.IsObject()) {
        std::cout << "Account Ingest Error 1: Account (Object) is of type " << kTypeNames[object.GetType()] << std::endl;
        return -1;
    }
    id = object["id"].GetString();
    username = object["username"].GetString();
    acct = object["acct"].GetString();
    url = object["url"].GetString();
    display_name = object["display_name"].GetString();
    // note
    // avatar
    // avatar_static
    // header
    // header_static
    // locked
    // fields
    // emojis
    // bot
    // group
    // discoverable
    // noindex optional
    // moved optional
    // suspended optional
    // limited optional
    // created_at
    // last_status_at
    // statuses_count
    // followers_count
    // following_count
    return 0;
}

int Status::Ingest(rapidjson::Value &object) {
    if (!object.IsObject()) {
        std::cout << "Status Ingest Error 1: Status (Object) is of type " << kTypeNames[object.GetType()] << std::endl;
        return -1;
    }
    id = object["id"].GetString();
    uri = object["uri"].GetString();
    created_at = read8601(object["created_at"].GetString());
    account.Ingest(object["account"]);
    content = object["content"].GetString();
    // visibility
    // sensitive
    // spoiler_text
    // media_attachments
    // application optional
    // mentions
    // tags
    // emojis
    // reblogs_count
    // favourites_count
    // replies_count
    // url
    // in_reply_to_id
    // in_reply_to_account_id
    // reblog
    // poll
    // card
    // language
    // text
    // edited_at
    // favourited optional
    // reblogged optional
    // muted optional
    // bookmarked optional
    // pinned optional
    // filtered optional
    return 0;
}
