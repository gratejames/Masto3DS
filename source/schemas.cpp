#include "schemas.hpp"

static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
static const char* kIngestTypeNames[] = { "Null", "Bool", "Object", "Array", "String", "Number" };

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

bool IngestCheckMemberExist(string typeName, rapidjson::Value &object, const char* key, bool optional = false) {
    if (!object.HasMember(key)) {
        if (!optional) {
            std::cout << ANSIerror << typeName << " ingest error:" << ANSIreset << " key " << key << " not found." << std::endl;
        }
        return false;
    }
    return true;
}

bool IngestCheckType(string typeName, rapidjson::Value &object, Ingest::CheckTypes type, string key="?", bool nullable = false) {
    bool valid;
    if (nullable && object.IsNull()) {
        return true;
    }
    switch (type) {
    case Ingest::Null:
        valid = object.IsNull();
        break;
    case Ingest::Bool:
        valid = object.IsBool();
        break;
    case Ingest::Object:
        valid = object.IsObject();
        break;
    case Ingest::Array:
        valid = object.IsArray();
        break;
    case Ingest::String:
        valid = object.IsString();
        break;
    case Ingest::Number:
        valid = object.IsNumber();
        break;
    default:
        valid = false;
    }
    if (!valid)
        std::cout << ANSIerror << typeName << " ingest error:" << ANSIreset << " key " << key << " (" << kIngestTypeNames[type] << ") was " << kTypeNames[object.GetType()] << std::endl;
    return valid;
}

bool IngestCheck(string typeName, rapidjson::Value &object, const char* key, Ingest::CheckTypes type, Ingest::CheckParams params = Ingest::None) {
    // Asserts that the object has member
    bool optional = params & Ingest::Optional;
    if (!IngestCheckMemberExist(typeName, object, key, optional)) {
        // It not existing is only a problem if it's not optional
        return optional;
    }
    // Asserts that the member is the correct type
    return IngestCheckType(typeName, object[key], type, key, params & Ingest::Nullable);
}

bool IngestCheck(string typeName, rapidjson::Value &object, const char* key, Ingest::CheckTypes type, int params) {
    return IngestCheck(typeName, object, key, type, (Ingest::CheckParams)params);
}
    
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
    // avatar = object["avatar"].GetString();
    avatar_static = object["avatar_static"].GetString();
    // header
    // header_static
    // locked
    // fields
    for (rapidjson::Value &emojiObj : object["emojis"].GetArray()) {
        CustomEmoji newEmoji = {};
        int retCode = newEmoji.Ingest(emojiObj);
        if (retCode != 0)
            return retCode;
        emojis.push_back(newEmoji);
    }
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

int CustomEmoji::Ingest(rapidjson::Value &object) {
    if (!IngestCheckType("CustomEmoji", object, Ingest::Object)) {return -1;}
    bool valid = true;
    valid = valid && IngestCheck("CustomEmoji", object, "shortcode",            Ingest::String);
    valid = valid && IngestCheck("CustomEmoji", object, "url",                  Ingest::String);
    valid = valid && IngestCheck("CustomEmoji", object, "static_url",           Ingest::String);
    valid = valid && IngestCheck("CustomEmoji", object, "visible_in_picker",    Ingest::Bool);
    valid = valid && IngestCheck("CustomEmoji", object, "category",             Ingest::String, Ingest::Nullable | Ingest::Optional); // Spec break: not marked as optional, but not given by mastodon.social

    if (!valid) {
        std::cout << "Invalid ingest" << std::endl;
        return -1;
    }

    shortcode           = object["shortcode"].GetString();
    url                 = object["url"].GetString();
    static_url          = object["static_url"].GetString();
    visible_in_picker   = object["visible_in_picker"].GetBool();
    category            = (object.HasMember("category") && object["category"].IsString()) ? object["category"].GetString() : "";

    return 0;
}

int Status::Ingest(rapidjson::Value &object) {
    if (!IngestCheckType("Status", object, Ingest::Object)) {return -1;}
    bool valid = true;
    valid = valid && IngestCheck("Status", object, "id",                        Ingest::String);
    valid = valid && IngestCheck("Status", object, "uri",                       Ingest::String);
    valid = valid && IngestCheck("Status", object, "created_at",                Ingest::String);
    valid = valid && IngestCheck("Status", object, "account",                   Ingest::Object);
    valid = valid && IngestCheck("Status", object, "content",                   Ingest::String);
    valid = valid && IngestCheck("Status", object, "visibility",                Ingest::String);
    valid = valid && IngestCheck("Status", object, "sensitive",                 Ingest::Bool);
    valid = valid && IngestCheck("Status", object, "spoiler_text",              Ingest::String);
    valid = valid && IngestCheck("Status", object, "media_attachments",         Ingest::Array);
    valid = valid && IngestCheck("Status", object, "application",               Ingest::Object, Ingest::Optional);
    if (object.HasMember("application")) {
        valid = valid && IngestCheck("Status", object["application"], "name",    Ingest::String);
        valid = valid && IngestCheck("Status", object["application"], "website", Ingest::String, Ingest::Nullable);
    }
    valid = valid && IngestCheck("Status", object, "mentions",                  Ingest::Array);
    valid = valid && IngestCheck("Status", object, "tags",                      Ingest::Array);
    valid = valid && IngestCheck("Status", object, "emojis",                    Ingest::Array);
    valid = valid && IngestCheck("Status", object, "reblogs_count",             Ingest::Number);
    valid = valid && IngestCheck("Status", object, "favourites_count",          Ingest::Number);
    valid = valid && IngestCheck("Status", object, "replies_count",             Ingest::Number);
    valid = valid && IngestCheck("Status", object, "url",                       Ingest::String, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "in_reply_to_id",            Ingest::String, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "in_reply_to_account_id",    Ingest::String, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "reblog",                    Ingest::Object, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "poll",                      Ingest::Object, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "card",                      Ingest::Object, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "language",                  Ingest::String, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "text",                      Ingest::String, Ingest::Nullable | Ingest::Optional); // Spec break: not marked as optional, but not given by mastodon.social
    valid = valid && IngestCheck("Status", object, "edited_at",                 Ingest::String, Ingest::Nullable);
    valid = valid && IngestCheck("Status", object, "favourited",                Ingest::Bool, Ingest::Optional);
    valid = valid && IngestCheck("Status", object, "reblogged",                 Ingest::Bool, Ingest::Optional);
    valid = valid && IngestCheck("Status", object, "muted",                     Ingest::Bool, Ingest::Optional);
    valid = valid && IngestCheck("Status", object, "bookmarked",                Ingest::Bool, Ingest::Optional);
    valid = valid && IngestCheck("Status", object, "pinned",                    Ingest::Bool, Ingest::Optional);

    if (!valid) {
        std::cout << "Invalid ingest" << std::endl;
        return -1;
    }

    id                  = object["id"].GetString();
    uri                 = object["uri"].GetString();
    created_at          = read8601(object["created_at"].GetString());
    account.Ingest(object["account"]);
    content             = object["content"].GetString();
    // visibility
    // sensitive
    // spoiler_text
    // media_attachments
    // application optional
    // mentions
    // tags
    for (rapidjson::Value &emojiObj : object["emojis"].GetArray()) {
        CustomEmoji newEmoji = {};
        int retCode = newEmoji.Ingest(emojiObj);
        if (retCode != 0)
            return retCode;
        emojis.push_back(newEmoji);
    }
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
