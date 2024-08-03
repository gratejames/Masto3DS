#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
using std::vector;
using std::string;

#define ANSIreset "\x1b[0m"
#define ANSIerror "\x1b[31m"

struct Ingest{
    enum CheckParams{
        None           = 0,
        Optional       = (1<<0),
        Nullable       = (1<<1),
    };
    enum CheckTypes {
        Null,
        Bool,
        Object,
        Array,
        String,
        Number,
    };
};

enum StatusVisibility {
    SV_Public,          // Visible to everyone, shown in public timelines.
    SV_Unlisted,        // Visible to public, but not included in public timelines.
    SV_Private,         // Visible to followers only, and to any mentioned users.
    SV_Direct           // Visible only to mentioned users.
};

enum MediaAttachmentType {
    MAT_Unknown,        // unsupported or unrecognized file type
    MAT_Image,          // Static image
    MAT_Gifv,           // Looping, soundless animation
    MAT_Video,          // Video clip
    MAT_Audio           // Audio track
};

enum PreviewCardType {
    PCT_Link,           // Link OEmbed
    PCT_Photo,          // Photo OEmbed
    PCT_Video,          // Video OEmbed
    PCT_Rich            // iframe OEmbed. Not currently accepted, so won’t show up in practice.
};

enum FilterContext {
    FC_Home,            // home timeline and lists
    FC_Notifications,   // notifications timeline
    FC_Public,          // public timelines
    FC_Thread,          // expanded thread of a detailed status
    FC_Account          // when viewing a profile
};

enum FilterAction {
    FA_Warn,            // show a warning that identifies the matching filter by title,
                        // and allow the user to expand the filtered status.
                        // This is the default (and unknown values should be treated as equivalent to warn).
    FA_Hide,            // do not show this status if it is received
};

class Field {
public:
    int Ingest(rapidjson::Value &object);
    string name;
    string value;
    tm verified_at;
private:
};

class CustomEmoji {
public:
    int Ingest(rapidjson::Value &object);
    string shortcode;
    string url;
    string static_url;
    bool visible_in_picker;
    string category;
private:
};

class Account {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string username;
    string acct;
    string url;
    string display_name;
    string note;
    string avatar;
    string avatar_static;
    string header;
    string header_static;
    bool locked;
    vector<Field> fields;
    vector<CustomEmoji> emojis;
    bool bot;
    bool group;
    bool discoverable;
    bool noindex;
    Account* moved;
    bool suspended;
    bool limited;
    tm created_at;
    tm last_status_at;
    int statuses_count;
    int followers_count;
    int following_count;

private:
};

class MediaAttachment {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    MediaAttachmentType type;
    string url;
    string preview_url;
    string remote_url;
    bool meta; // TODO: Find correct type
    string description;
    string blurhash;
private:
};

class Status_Mention {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string username;
    string url;
    string acct;
private:
};

class Status_Tag {
public:
    int Ingest(rapidjson::Value &object);
    string name;
    string url;
private:
};

class Poll_Option {
public:
    int Ingest(rapidjson::Value &object);
    string title;
    int votes_count;
private:
};

class Poll {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    tm expires_at;
    bool expired;
    bool multiple;
    int votes_count;
    int voters_count;
    vector<Poll_Option> options;
    vector<CustomEmoji> emojis;
    bool voted;
    vector<int> own_votes;
private:
};

class PreviewCard {
public:
    int Ingest(rapidjson::Value &object);
    string url;
    string title;
    string description;
    PreviewCardType type;
    string author_name;
    string author_url;
    string provider_name;
    string provider_url;
    string html;
    int width;
    int height;
    string image;
    string embed_url;
    string blurhash;
private:
};

class FilterKeywords {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string keyword;
    bool whole_word;
private:
};

class FilterStatus {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string status_id;
private:
};

class Filter {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string title;
    FilterContext context;
    tm expires_at;
    FilterAction filter_action;
    vector<FilterKeywords> keywords;
    vector<FilterStatus> statuses;
private:
};

class FilterResult {
public:
    int Ingest(rapidjson::Value &object);
    Filter filter;
    vector<string> keyword_matches;
    vector<string> status_matches;
private:
};

class Status {
public:
    int Ingest(rapidjson::Value &object);
    string id;
    string uri;
    tm created_at;
    Account account;
    string content;
    StatusVisibility visibility;
    bool sensitive;
    string spoiler_text;
    vector<MediaAttachment> media_attachments;
    string application_name;
    string application_website;
    vector<Status_Mention> mentions;
    vector<Status_Tag> tags;
    vector<CustomEmoji> emojis;
    int reblogs_count;
    int favourites_count;
    int replies_count;
    string url;
    string in_reply_to_id;
    string in_reply_to_account_id;
    Status* reblog;
    Poll poll;
    PreviewCard card;
    string language;
    string text;
    tm edited_at;
    bool favourited;
    bool reblogged;
    bool muted;
    bool bookmarked;
    bool pinned;
    vector<FilterResult> filtered;
private:
};

class Timeline {
public:
    int Ingest(rapidjson::Document &doc);
    vector<Status> statuses;
private:
};