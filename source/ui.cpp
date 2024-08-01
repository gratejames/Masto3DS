#include "ui.hpp"

string getUTF8Char(string text, uint index) {
    // TODO: Assert that following bytes match 10xxxxxx
    // https://stackoverflow.com/questions/44565859/how-does-utf-8-encoding-identify-single-byte-and-double-byte-characters
    u8 byte1 = text[index];
    int charLength = 0;
    if (byte1 < 0b11000000) {
        charLength = 1;
    } else if (byte1 < 0b11100000) {
        charLength = 2;
    } else if (byte1 < 0b11110000) {
        charLength = 3;
    } else {
        charLength = 4;
    }
    string UTF8Char = text.substr(index, charLength);
    return UTF8Char;
}

text::text(string dispText, u32 color, float scale, vector<CustomEmoji> emojis) {
    this->dispText = dispText;
    this->color = color;
    this->scale = scale;
    this->emojis = emojis;
}

void text::Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth) {
    float t_width = 0, t_height = 0;
    width = 0, height = 0;

    C2D_TextBuf textBuf = C2D_TextBufNew(40);
    C2D_Text c2text;
    uint i = 0;
    while (i < dispText.length()) {
        string utf8Char = getUTF8Char(dispText, i);
        i += utf8Char.length();
        if (utf8Char == ":") {
            // std::cout << "Begin Emoji!" << std::endl;
            // Begin emoji!!
            string emojiLookAhead = "";
            vector<pair<int, string>> emojiNames = {};
            int longestEmojiCode = 0;
            for (uint iEmoji = 0; iEmoji < emojis.size(); iEmoji++) {
                // std::cout << iEmoji << " emoji: " << emojis[iEmoji].shortcode << std::endl;
                emojiNames.push_back({iEmoji, emojis[iEmoji].shortcode});
                longestEmojiCode = std::max(longestEmojiCode, (int)emojis[iEmoji].shortcode.length());
            }
            // std::cout << "Listed!" << std::endl;
            // Emoji's are in a list. Reduce it somehow?
            int lookAheadIndex = 1;
            // Loop while we either have multiple emoji options and the lookahead index is less than the longest emoji
            while (emojiNames.size() > 1 && lookAheadIndex < longestEmojiCode) {
                string newChar = getUTF8Char(dispText, i + lookAheadIndex);
                if (newChar ==  ":") {
                    break;
                }
                emojiLookAhead.append(newChar);
                for (uint emojiNameIndex = 0; emojiNameIndex < emojiNames.size(); ) {
                    string shortcode = emojiNames[emojiNameIndex].second;
                    // Check if each emoji shortcode begins with whatever
                    // partial we are constructing
                    // Start searching at the beginning (working towards the left) and only accept the beginning.
                    if (!(shortcode.rfind(emojiLookAhead, 0) == 0)) {
                        // No match!
                        std::cout << shortcode << " not matches LA " << emojiLookAhead << std::endl;
                        // We advance to the next by removing the current one and letting a new one replace it
                        emojiNames.erase(emojiNames.begin() + emojiNameIndex);
                    } else {
                        std::cout << shortcode << " matches LA " << emojiLookAhead << std::endl;
                        // We advance by incrememnting
                        emojiNameIndex++;
                    }
                }
                lookAheadIndex++;
                break;
            }
            // std::cout << "Culled!" << std::endl;
            // We now have emojiNames with either 0 or 1 items, hopefully
            if (emojiNames.size() == 0) {
                std::cout << "No emoji found for (LA) shortcode " << emojiLookAhead << std::endl;
            } else if (emojiNames.size() == 1) {
                // Show that the shortcode left in the list matches
                // the shortcode that we can pull from the original emojis list
                // std::cout << "Found: " << emojiNames[0].second << "=" << emojis[emojiNames[0].first].shortcode << std::endl;
                if (dispText.length() < i + emojiNames[0].second.length()) {
                    std::cout << "Out of room" << std::endl;
                    continue;
                }
                string possibleEmoji = dispText.substr(i, emojiNames[0].second.length());
                if (possibleEmoji != emojiNames[0].second) {
                    std::cout << "Mismatch" << std::endl;
                    continue;
                }
                std::cout << "Found: " << emojiNames[0].second << std::endl;
                // TODO: Download image from emojis[emojiNames[0].first].static_url
                // into memory and display it at the correct position, and advance
                // the linewidth by the width of the emoji
                i += emojiNames[0].second.length() + 1;
                
            } else {
                std::cout << "Emojis found n>1? n=" << emojiNames.size() << std::endl;
            }
            continue;
        }
        C2D_TextParse(&c2text, textBuf, utf8Char.c_str());
        C2D_TextOptimize(&c2text);
        C2D_TextGetDimensions(&c2text, scale, scale, &t_width, &t_height);
        C2D_DrawText(&c2text, C2D_WithColor, origin_x + width, origin_y, 0, scale, scale, color);
        width += t_width;
        height = std::max(t_height, height);
    }
    C2D_TextBufDelete(textBuf);

    if (uiDebug_textOutlines) {
        int x1 = origin_x;
        int x2 = origin_x + width;
        int y1 = origin_y;
        int y2 = origin_y + height;
        C2D_DrawLine(x1, y1, color_debug, x2, y1, color_debug, 1.2, 0);
        C2D_DrawLine(x1, y1, color_debug, x1, y2, color_debug, 1.2, 0);
        C2D_DrawLine(x2, y2, color_debug, x2, y1, color_debug, 1.2, 0);
        C2D_DrawLine(x2, y2, color_debug, x1, y2, color_debug, 1.2, 0);
    }
}

htmltext::htmltext(string dispText, float scale, vector<CustomEmoji> emojis) {
    this->dispText = dispText;
    this->scale = scale;
    this->emojis = emojis;
}

void htmltext::Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth) {
    const float lineHeight = scale * defaultFontHeight;
    float t_width = 0, t_height = 0;
    float currentLineWidth = 0;;
    width = 0, height = 0;

    C2D_TextBuf textBuf = C2D_TextBufNew(600);
    if (textBuf == NULL) {
        std::cout << "Text buf null..." << std::endl;
        return;
    }
    C2D_Text c2text;
    uint i = 0;
    stack<string> HTMLcontext;
    vector<string> HTMLproperties;
    string HTMLtagPartial;
    string HTMLpropertyKeyPartial;
    string HTMLpropertyValuePartial;

    bool readingTag = false; // Are we reading a tag?
    bool closingTag = false; // Is it a closing tag?
    bool readingProperty = false; // Are on to reading the property?
    bool readyForPropertyValue = false; // Have we passed the equals sign?
    bool readingPropertyValue = false; // Are on to reading the value?

    bool style_link = false;
    bool style_invis = false;
    std::cout << "BadHTML size " << dispText.length() << std::endl;
    while (i < dispText.length()) {
        string utf8Char = getUTF8Char(dispText, i);
        i += utf8Char.length();

        if (readingTag) {
            if (utf8Char == ">") {
                // At the end of a tag, close out the tag and properties
                readingTag = false;
                readingProperty = false;
                if (closingTag) {
                    // TODO: make sure that top exists. If there are more closing tags than opening tags, this might be an issue
                    // Another alternative might be to add a dummy tag at the initalization of the stack so that too many 
                    // pop()s will expose it, inevitably triggering a mismatch tag error instead. Then we can check that this
                    // dummy is the top() at the very end to prevent/warn against dangling tags
                    if (HTMLcontext.top() != HTMLtagPartial) {
                        std::cout << "Error in BadHTML: closing tag " << HTMLtagPartial << " does not match .top() " << HTMLcontext.top() << std::endl;
                        std::cout << "Breaking..." << std::endl;
                        break;
                    }
                    // std::cout << "-" << HTMLtagPartial << "-";
                    if (HTMLtagPartial == "p") { // On closing p tag, \n
                        height += lineHeight;
                        currentLineWidth = 0;
                    } else if (HTMLtagPartial == "a") {
                        // currentColor = color_text;
                        style_link = false;
                    }
                    style_invis = false; // On closing reset invis
                    HTMLcontext.pop();
                    closingTag = false;
                } else {
                    // std::cout << "+" << HTMLtagPartial << "+";
                    if (HTMLtagPartial == "br") { // On opening br tag, don't push (self closing) and \n
                        height += lineHeight;
                        currentLineWidth = 0;
                    } else {
                        HTMLcontext.push(HTMLtagPartial);
                    }
                    if (HTMLtagPartial == "a") { // On opening link tag, set color to link color
                        // currentColor = color_textLink;
                        style_link = true;
                    }
                }
                HTMLtagPartial = "";
                if (readingProperty) {
                    HTMLproperties.push_back(HTMLpropertyKeyPartial);
                    // std::cout << "Prop(" << HTMLpropertyKeyPartial << ")";
                    HTMLpropertyKeyPartial = "";
                }
            } else if (utf8Char == "/" && HTMLtagPartial == "") {
                // Slash, and tag partial is empty so we're at the beginning.
                // That makes this a closing tag
                closingTag = true;
            } else if (utf8Char == "\"") {
                // Either the opening or closing quote of a property
                // If it's the opengin quote, then 
                if (!readingProperty) {
                    // We've hit a quote and we're still in the tag body!
                    std::cout << "Error in BadHTML: quote before property" << std::endl;
                    std::cout << "Breaking..." << std::endl;
                    break;
                }
                std::cout << "";
                if (readyForPropertyValue) {
                    if (readingPropertyValue) {
                        // If we're already reading, this is a closing quote
                        // std::cout << "(" << HTMLpropertyKeyPartial << ":" << HTMLpropertyValuePartial << ")";
                        if (HTMLpropertyKeyPartial == "class" && (HTMLpropertyValuePartial.find("invisible") != std::string::npos)) {
                            // std::cout << "(Invis)";
                            style_invis = true;
                        }
                        HTMLpropertyKeyPartial = "";
                        HTMLpropertyValuePartial == "";
                        readingPropertyValue = false;
                        readyForPropertyValue = false;
                    } else {
                        // If we haven't started reading, let's get ready to
                        readingPropertyValue = true;
                        HTMLpropertyValuePartial = "";
                    }
                } else {
                    // We've hit a quote before we've hit the arrow
                    std::cout << "Error in BadHTML: property value before ready" << std::endl;
                    std::cout << "Breaking..." << std::endl;
                    break;
                }
            } else if (utf8Char == "=") {
                // Seperator of the property and its value
                // Prepare the propery value
                readyForPropertyValue = true;
            } else if (utf8Char == " ") {
                // Either between proprties, or in quotes 
                // If we're not reading a property, then this means that we now are.
                // If we already are, then whitespace is largely irrelevant
                if (!readingProperty) {
                    HTMLpropertyKeyPartial = "";
                    readingProperty = true;
                }
                // Actually, we need to keep track of the whitespace if we're in a value...
                if (readingProperty && readingPropertyValue) {
                    HTMLpropertyValuePartial.append(utf8Char);
                }
            } else {
                if (readingProperty) {
                    if (readingPropertyValue) {
                        HTMLpropertyValuePartial.append(utf8Char);
                    } else {
                        HTMLpropertyKeyPartial.append(utf8Char);
                    }
                } else {
                    HTMLtagPartial.append(utf8Char);
                }
            }
            continue;
        }

        if (utf8Char == "<") {
            readingTag = true;
            continue;
        }    

        if (style_invis)
            continue;

        C2D_TextParse(&c2text, textBuf, utf8Char.c_str());
        C2D_TextOptimize(&c2text);
        C2D_TextGetDimensions(&c2text, scale, scale, &t_width, &t_height);
        // TODO: Some sort of word wrapping? Pehaps scan ahead to the next space when we hit a space, then check if the word can fit in?
        if (currentLineWidth + t_width > maxWidth) {
            height += lineHeight;
            currentLineWidth = 0;
        }
        float text_x = origin_x + currentLineWidth;
        float text_y = origin_y + height;
        if (style_link) {
            C2D_DrawText(&c2text, C2D_WithColor, text_x, text_y, 0, scale, scale, color_textLink);
            // Round the underlining to the nearest 2 for some reason? Otherwise it can dissapear
            int underline_y = text_y + lineHeight - 2;
            C2D_DrawLine(text_x, underline_y, color_textLink, text_x + t_width, underline_y, color_textLink, 1, 0);
        } else {
            C2D_DrawText(&c2text, C2D_WithColor, text_x, text_y, 0, scale, scale, color_text);
        }        
        currentLineWidth += t_width;
        width = std::max(width, currentLineWidth);
    }
    C2D_TextBufDelete(textBuf);
    // std::cout << "BadHTML Done!" << std::endl;


    if (uiDebug_textOutlines) {
        int x1 = origin_x;
        int x2 = origin_x + width;
        int y1 = origin_y;
        int y2 = origin_y + height;
        C2D_DrawLine(x1, y1, color_debug, x2, y1, color_debug, 1.2, 0);
        C2D_DrawLine(x1, y1, color_debug, x1, y2, color_debug, 1.2, 0);
        C2D_DrawLine(x2, y2, color_debug, x2, y1, color_debug, 1.2, 0);
        C2D_DrawLine(x2, y2, color_debug, x1, y2, color_debug, 1.2, 0);
    }
}

uiStatus::uiStatus(Status &status) : internalStatus(status) {
    display_name = text(internalStatus.account.display_name, color_text, 0.5, internalStatus.account.emojis);
    acct = text(internalStatus.account.acct, color_text, 0.4, internalStatus.account.emojis);
    content = htmltext(internalStatus.content, 0.4, status.emojis);
}

void uiStatus::Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth) {
    float t_width, t_height;

    // Padding
    origin_y += uiPad;
    origin_x += uiPad;

    // Draw display_name
    display_name.Draw(origin_x, origin_y, t_width, t_height, maxWidth);

    float acct_x = origin_x + t_width + uiPad;
    float name_height = t_height;

    // Since the acct is slightly smaller than the display name
    // and we want their baseliens aligned.
    // We need to bump the acct down slightly
    // The magnitude of the bump is the difference in font sizes.
    // We can find the height of the acct by multiplying the
    // scale by the height.
    float acct_y = origin_y + (name_height - acct.scale*defaultFontHeight);

    // Draw acct
    acct.Draw(acct_x, acct_y, t_width, t_height, maxWidth);

    // Pick max of display_name height and acct height
    name_height = std::max(name_height, t_height);

    // Draw content
    content.Draw(origin_x, origin_y + name_height + uiPad, t_width, t_height, maxWidth);

    // Set height of this item: padding + content_y + content_height + padding
    height = uiPad + name_height + uiPad + t_height + uiPad;

    // std::cout << origin_y << ":" << height << std::endl;
}