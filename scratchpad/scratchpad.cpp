#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <vector>
#include <inttypes.h>
using std::list;
using std::string;
using std::stack;
using std::vector;

typedef  unsigned char      u8;
// typedef uint32_t u32;

// vector<unsigned char> out = {
//     0xff, 0x00, 0x00, 0xff,    0xff, 0x00, 0x00, 0xff,   0xff, 0x00, 0x00, 0xff,   0xff, 0x00, 0x00, 0xff,
//     0x00, 0xff, 0x00, 0xff,    0x00, 0xff, 0x00, 0xff,   0x00, 0xff, 0x00, 0xff,   0x00, 0xff, 0x00, 0xff,
//     0x00, 0x00, 0xff, 0xff,    0x00, 0x00, 0xff, 0xff,   0x00, 0x00, 0xff, 0xff,   0x00, 0x00, 0xff, 0xff,
//     0xff, 0xff, 0xff, 0xff,    0xff, 0xff, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff,
// };

// uint width = 4;
// uint height = 4;

// vector<unsigned char> imagePix;

// u32 C2D_Color32(uint r, uint g, uint b, uint a) {
//     return (r << 24) + (g << 16) + (b << 8) + (a << 0);
// }

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

int main() {
    string emoji = "\"🇨🇦\"";
    for (int i = 0; i < emoji.length();) {
        string c = getUTF8Char(emoji, i);
        i += c.length();
        std::cout << c << "|";
    }
    // for (uint i = 0; i < out.size()/4; i+=1) {
    //     // HAHAHAHA DESWIZZLE IT'S BEEN DAYS HELP ME
    //     u32 x = 0, y = 0;
    //     uint bitPos = 0;
    //     u32 i2 = i;
    //     while (i2 != 0) {
    //         if (bitPos%2 == 0)
    //             // Even bits
    //             y += (i2%2) << (bitPos/2);
    //         else
    //             // Odd bits
    //             x += (i2%2) << (bitPos/2);
    //         bitPos++;
    //         i2 = i2 >> 1;
    //     }
            
    //     x = width - 1 - x;
    //     uint finalIndex = (y*width + x)*4;
    //     std::cout << std::hex << C2D_Color32(out[finalIndex+3],out[finalIndex+2],out[finalIndex+1],out[finalIndex+0]) << "|";
    //     // imagePix.push_back(C2D_Color32(out[finalIndex+3],out[finalIndex+2],out[finalIndex+1],out[finalIndex+0]));
    // }
    // std::cout << imagePix < std::endl;
}

// uint genSwizzleNumbers(uint n, bool genX) {
//     uint bitPos = 0;
//     uint x = 0;
//     while (n != 0) {
//         x += (n%2) << ((2 * bitPos) + genX);
//         n = n >> 1;
//         bitPos += 1;
//     }
//     return x;
// }

// int main() {
//     for (int i = 0; i < 16; i++) {
//         std::cout << genSwizzleNumbers(i, true) << std::endl;
//     }
// }

// string getUTF8Char(string text, uint &index) {
//     // TODO: Assert that following bytes match 10xxxxxx
//     // https://stackoverflow.com/questions/44565859/how-does-utf-8-encoding-identify-single-byte-and-double-byte-characters
//     u8 byte1 = text[index];
//     int charLength = 0;
//     if (byte1 < 0b11000000) {
//         charLength = 1;
//     } else if (byte1 < 0b11100000) {
//         charLength = 2;
//     } else if (byte1 < 0b11110000) {
//         charLength = 3;
//     } else {
//         charLength = 4;
//     }
//     string UTF8Char = text.substr(index, charLength);
//     index += charLength;
//     return UTF8Char;
// }

// int main() {
//     string dispText = ""
//     "<p>"
//         "$\\alpha$ Annealing of Ant Colony Optimization in the infinite-range Ising model"
//     "</p>"
//     "<p>"
//         "Shintaro Mori, Taiyo Shimizu, Masato Hisakado, Kazuaki Nakayama"
//         "<br>"
//         "<a href=\"https://arxiv.org/abs/2407.19245\" rel=\"nofollow noopener noreferrer\" translate=\"no\" target=\"_blank\">"
//             "<span class=\"invisible\">https://</span>"
//             "<span class=\"\">arxiv.org/abs/2407.19245</span>"
//             "<span class=\"invisible\"></span>"
//         "</a>"
//         "<a href=\"https://arxiv.org/pdf/2407.19245\" rel=\"nofollow noopener noreferrer\" translate=\"no\" target=\"_blank\">"
//             "<span class=\"invisible\">https://</span>"
//             "<span class=\"\">arxiv.org/pdf/2407.19245</span>"
//             "<span class=\"invisible\"></span>"
//         "</a>"
//     "</p>"
//     "<p>"
//         "arXiv:2407.19245v1 Announce Type: new "
//         "<br>"
//         "Abstract: Ant colony optimization (ACO) leverages the parameter $\\alpha$ to modulate the "
//         "decision function's sensitivity to pheromone levels, balancing the exploration of diverse "
//         "solutions with the exploitation of promising areas. Identifying the optimal value for $\\alpha$ "
//         "and establishing an effective annealing schedule remain significant challenges, particularly "
//         "in complex optimization scenarios. This study investigates the $\\alpha$-annealing process of "
//         "the linear Ant System within the infinite-range Ising model to address these challenges. Here, "
//         "\"linear\" refers to the decision function employed by the ants. By systematically increasing "
//         "$\\alpha$, we explore its impact on enhancing the search for the ground state. We derive the "
//         "Fokker-Planck equation for the pheromone ratios and obtain the joint probability density "
//         "function (PDF) in stationary states. As $\\alpha$ increases, the joint PDF transitions from a "
//         "mono-modal to a multi-modal state. In the homogeneous fully connected Ising model, "
//         "$\\alpha$-annealing facilitates the transition from a trivial solution at $\\alpha=0$ to the "
//         "ground state. The parameter $\\alpha$ in the annealing process plays a role analogous to the "
//         "transverse field in quantum annealing. Our findings demonstrate the potential of "
//         "$\\alpha$-annealing in navigating complex optimization problems, suggesting its broader "
//         "application beyond the infinite-range Ising model."
//     "</p>";
//     uint i = 0;
//     stack<string> HTMLcontext;
//     list<string> HTMLproperties;
//     string HTMLtagPartial;
//     string HTMLpropertyKeyPartial;
//     string HTMLpropertyValuePartial;

//     bool readingTag = false; // Are we reading a tag?
//     bool closingTag = false; // Is it a closing tag?
//     bool readingProperty = false; // Are on to reading the property?
//     bool readyForPropertyValue = false; // Have we passed the equals sign?
//     bool readingPropertyValue = false; // Are on to reading the value?
//     while (i < dispText.length()) {
//         string utf8Char = getUTF8Char(dispText, i);

//         if (readingTag) {
//             if (utf8Char == ">") {
//                 // At the end of a tag, close out the tag and properties
//                 readingTag = false;
//                 readingProperty = false;
//                 if (closingTag) {
//                     if (HTMLcontext.top() != HTMLtagPartial) {
//                         std::cout << "Error in HTML parser: closing tag " << HTMLtagPartial << " does not match .top() " << HTMLcontext.top() << std::endl;
//                         std::cout << "Breaking..." << std::endl;
//                         break;
//                     }
//                     std::cout << "-" << HTMLtagPartial << "-";
//                     if (HTMLtagPartial == "p") { // On closing br tag, \n
//                         std::cout << std::endl;
//                     }
//                     HTMLcontext.pop();
//                     closingTag = false;
//                 } else {
//                     if (HTMLtagPartial == "br") { // On opening br tag, \n
//                         std::cout << std::endl;
//                     }
//                     std::cout << "+" << HTMLtagPartial << "+";
//                     if (HTMLtagPartial == "br") { // On opening br tag, don't push (self closing) and \n
//                         std::cout << std::endl;
//                     } else {
//                         HTMLcontext.push(HTMLtagPartial);
//                     }
//                 }
//                 HTMLtagPartial = "";
//                 if (readingProperty) {
//                     HTMLproperties.push_back(HTMLpropertyKeyPartial);
//                     // std::cout << "Prop(" << HTMLpropertyKeyPartial << ")";
//                     HTMLpropertyKeyPartial = "";
//                 }
//             } else if (utf8Char == "/" && HTMLtagPartial == "") {
//                 // Slash, and tag partial is empty so we're at the beginning.
//                 // That makes this a closing tag
//                 closingTag = true;
//             } else if (utf8Char == "\"") {
//                 // Either the opening or closing quote of a property
//                 // If it's the opengin quote, then 
//                 if (!readingProperty) {
//                     // We've hit a quote and we're still in the tag body!
//                     std::cout << "Error in HTML parser: quote before property" << std::endl;
//                     std::cout << "Breaking..." << std::endl;
//                     break;
//                 }
//                 std::cout << "";
//                 if (readyForPropertyValue) {
//                     if (readingPropertyValue) {
//                         // If we're already reading, this is a closing quote
//                         std::cout << "(" << HTMLpropertyKeyPartial << ":" << HTMLpropertyValuePartial << ")";
//                         HTMLpropertyKeyPartial = "";
//                         HTMLpropertyValuePartial == "";
//                         readingPropertyValue = false;
//                         readyForPropertyValue = false;
//                     } else {
//                         // If we haven't started reading, let's get ready to
//                             readingPropertyValue = true;
//                             HTMLpropertyValuePartial = "";
//                     }
//                 } else {
//                     // We've hit a quote before we've hit the arrow
//                     std::cout << "Error in HTML parser: property value before ready" << std::endl;
//                     std::cout << "Breaking..." << std::endl;
//                     break;
//                 }
//             } else if (utf8Char == "=") {
//                 // Seperator of the property and its value
//                 // Prepare the propery value
//                 readyForPropertyValue = true;
//             } else if (utf8Char == " ") {
//                 // Either between proprties, or in quotes 
//                 // If we're not reading a property, then this means that we now are.
//                 // If we already are, then whitespace is largely irrelevant
//                 if (!readingProperty) {
//                     HTMLpropertyKeyPartial = "";
//                     readingProperty = true;
//                 }
//                 // Actually, we need to keep track of the whitespace if we're in a value...
//                 if (readingProperty && readingPropertyValue) {
//                     HTMLpropertyValuePartial.append(utf8Char);
//                 }
//             } else {
//                 if (readingProperty) {
//                     if (readingPropertyValue) {
//                         HTMLpropertyValuePartial.append(utf8Char);
//                     } else {
//                         HTMLpropertyKeyPartial.append(utf8Char);
//                     }
//                 } else {
//                     HTMLtagPartial.append(utf8Char);
//                 }
//             }
//             continue;
//         }

//         if (utf8Char == "<") {
//             readingTag = true;
//             continue;
//         }

//         std::cout << "" << utf8Char << "";
//     }
// }
