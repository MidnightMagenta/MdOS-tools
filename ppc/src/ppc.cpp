#include <CLI11.hpp>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <regex>
#include <string>

using json = nlohmann::json;

int process_file(const json &db, std::ifstream &inf, std::ofstream &outf) {
    std::stringstream inFileBuff;
    inFileBuff << inf.rdbuf();
    std::string infBuff = inFileBuff.str();

    std::regex token_re(R"(\/\*@([a-zA-Z0-9_-]*)@\*\/)");
    std::sregex_iterator it(infBuff.begin(), infBuff.end(), token_re);
    std::sregex_iterator end_it;

    size_t lastPos = 0;
    std::string out;

    for (; it != end_it; ++it) {
        const std::smatch &m = *it;
        out.append(infBuff, lastPos, m.position() - lastPos);
        std::string name = m[1].str();

        if (!db.contains(name)) {
            std::cerr << "Invalid token: " << name << "\n";
            return EXIT_FAILURE;
        }

        try {
            out += db.at(name).get<std::string>();
        } catch (const std::exception &e) {
            std::cerr << "Error retrieving token value for: " << name << " (" << e.what() << ")\n";
            return EXIT_FAILURE;
        }

        lastPos = m.position() + m.length();
    }

    out.append(infBuff, lastPos, std::string::npos);

    outf.write(out.data(), out.size());
    if (!outf) {
        std::cerr << "Error writing to output file\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	std::string databsePath;
	std::string targetPath;
	std::string outPath;

	CLI::App app;
	app.add_option("-d,--database", databsePath, "Used to specify the token replacement database")->required();
	app.add_option("-i,--in", targetPath, "Used to specify the input file")->required();
	app.add_option("-o,--out", outPath, "Used to specify the output path")->required();

	CLI11_PARSE(app, argc, argv);
	if (!app.parsed()) { return EXIT_FAILURE; }

	std::ifstream dbf(databsePath);
	if (!dbf.is_open()) { return EXIT_FAILURE; }
	json db = json::parse(dbf);

	std::ifstream inFile(targetPath);
	if (!dbf.is_open()) {
		dbf.close();
		return EXIT_FAILURE;
	}

	std::ofstream outFile(outPath);
	if (!dbf.is_open()) {
		dbf.close();
		inFile.close();
		return EXIT_FAILURE;
	}

	int res = process_file(db, inFile, outFile);

	dbf.close();
	inFile.close();
	outFile.close();
}