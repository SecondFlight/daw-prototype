#include "projectfile.h"
#include "exceptions.h"

#include "Include/rapidjson/filereadstream.h"
#include "Include/rapidjson/filewritestream.h"
#include "Include/rapidjson/writer.h"
#include "Include/rapidjson/schema.h"

ProjectFile::ProjectFile(QObject* parent) : QObject(parent) {
    dirty = false;

    // There's probably a Better Way
    // TODO: Add model constructors that give back a emtpy but valid state
    auto emptyProject =
        "{"
        "    \"software_version\": \"0.0.1\","
        "    \"project\": {"
        "        \"song\": {"
        "            \"patterns\": [],"
        "            \"arrangements\": []"
        "        },"
        "        \"transport\": {"
        "            \"master_pitch\": {"
        "                \"id\": 0,"
        "                \"initial_value\": 0,"
        "                \"minimum\": 12,"
        "                \"maximum\": -12,"
        "                \"step\": 1,"
        "                \"connection\": null,"
        "                \"override_automation\": false"
        "            }"
        "        },"
        "        \"mixer\": {},"
        "        \"generators\": []"
        "    }"
        "}";

    document.Parse(emptyProject);
}

ProjectFile::ProjectFile(QObject* parent, QString path) : QObject(parent) {
    // Below are JSON schemas (https://json-schema.org/) for validating project files. Use another editor to modify them.
    auto schema_0_0_1 = R"(
        {"$schema":"http://json-schema.org/draft-04/schema#","type":"object","properties":{"software_version":{"type":"string","pattern":"0.0.1"},"project":{"type":"object","properties":{"song":{"type":"object","properties":{"patterns":{"type":"array"},"arrangements":{"type":"array"}}},"transport":{"type":"object","properties":{"master_pitch":{"$ref":"#/definitions/control"}},"required":["master_pitch"]},"mixer":{"type":"object"},"generators":{"type":"array"}},"required":["transport"]}},"required":["software_version","project"],"title":"The Root Schema","definitions":{"control":{"type":"object","properties":{"id":{"type":"number"},"initial_value":{"type":"number"},"minimum":{"type":"number"},"maximum":{"type":"number"},"step":{"type":"number"},"connection":{"type":"null"},"override_automation":{"type":"boolean"}},"required":["id","initial_value","minimum","maximum","step","connection","override_automation"]}}}
    )";


    dirty = false;
    this->path = path;


    // Attempt to load the file as JSON

    bool isWindows = QSysInfo::kernelType() == "winnt";

    FILE* fp = std::fopen(path.toUtf8(), isWindows ? "rb" : "r");

    char readBuffer[65536];
    rapidjson::FileReadStream stream(fp, readBuffer, sizeof(readBuffer));

    document.ParseStream(stream);


    // Check for invalid JSON

    if (document.HasParseError()) {
        fclose(fp);
        throw InvalidProjectException("This project is corrupted and could not be opened.");
    }

    const char* schemaForValidation;


    // Check if software version in project matches current software version

    if (document.HasMember("software_version")) {
        QString version = document["software_version"].GetString();
        // Current version
        if (version == "0.0.1") {
            schemaForValidation = schema_0_0_1;
        }
        // Newer or unrecognized version
        else {
            fclose(fp);
            auto error = "This project was made with a newer version of Anthem (" + version.toStdString() + ") and could not be opened.";
            throw InvalidProjectException(error.c_str());
        }
    }
    else {
        fclose(fp);
        throw InvalidProjectException("This project is corrupted and could not be opened.");
    }


    // Validate project JSON against schema

    rapidjson::Document sd;
    sd.Parse(schemaForValidation);
    if (sd.HasParseError()) {
        fclose(fp);
        throw InvalidProjectException("Schema is invalid. This is a bug; please report it on Github.");
    }
    rapidjson::SchemaDocument schema(sd);

    // TODO: user-friendly but still informative error message
    rapidjson::SchemaValidator validator(schema);
    if (!document.Accept(validator)) {
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        rapidjson::StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        QString err = "This project is corrupted and could not be loaded.";
        // For debugging:
        /*
        err += "\n";
        err += "Invalid schema: ";
        err += sb.GetString();
        err += "\n";
        err += "Invalid keyword: ";
        err += validator.GetInvalidSchemaKeyword();
        err += "\n";
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        err += "Invalid document: ";
        err += sb.GetString();
        err += "\n";
        */
        throw InvalidProjectException(err);
    }

    fclose(fp);
}

void ProjectFile::save() {
    bool isWindows = QSysInfo::kernelType() == "winnt";
    FILE* fp = std::fopen((path).toUtf8(), isWindows ? "wb" : "w");
    char writeBuffer[65536];
    rapidjson::FileWriteStream stream(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(stream);
    document.Accept(writer);
    fclose(fp);
    markClean();
}

void ProjectFile::saveAs(QString path) {
    this->path = path;
    save();
}

void ProjectFile::markDirty() {
    dirty = true;
}

void ProjectFile::markClean() {
    dirty = false;
}

bool ProjectFile::isDirty() {
    return dirty;
}
