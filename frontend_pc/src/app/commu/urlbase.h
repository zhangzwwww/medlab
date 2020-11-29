#ifndef URLBASE_H
#define URLBASE_H

#include <QHash>

/* The file gives the definition of a base urls used in communication
 * Remeber to update this file once server address updates
 */

static QHash<QString, QString> urlbase{
    {"base", "http://42.193.8.10:5000"},
    {"base2", "http://42.193.8.10:5001"},
    {"base3", "http://42.193.8.10:5002"},
    {"auth", "/v1/auth"},
    {"patient", "/v1/patient"},
    {"image", "/v1/image"}
};

#endif // URLBASE_H
