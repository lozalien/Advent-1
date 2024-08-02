#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

// Class to handle calibration extraction
class CalibrationExtractor {
public:
	CalibrationExtractor(const string& filename);
	void process();
	int getPart1Sum() const;
	int getPart2Sum() const;

private:
	vector<string> lines;
	vector<int> part1Calibrations;
	vector<int> part2Calibrations;

	void extractPart1Calibrations();
	void extractPart2Calibrations();

	string convertToDigit(const string& value) const;
};

// Constructor to load the file
CalibrationExtractor::CalibrationExtractor(const string& filename) {
	ifstream inputFile(filename);
	if (!inputFile) {
		cerr << "Error opening file '" << filename << "'" << endl;
		exit(1);
	}

	string line;
	while (getline(inputFile, line)) {
		lines.push_back(line);
	}
	inputFile.close();
}

// Convert word to corresponding digit
string CalibrationExtractor::convertToDigit(const string& value) const {
	static const unordered_map<string, string> wordToDigit = {
		{"one", "1"}, {"two", "2"}, {"three", "3"}, {"four", "4"},
		{"five", "5"}, {"six", "6"}, {"seven", "7"}, {"eight", "8"},
		{"nine", "9"}, {"zero", "0"}
	};

	auto it = wordToDigit.find(value);
	if (it != wordToDigit.end()) {
		return it->second;
	}
	return value; // Return as is if not found
}

// Extract and process part 1 calibrations
void CalibrationExtractor::extractPart1Calibrations() {
	for (const string& line : lines) {
		string calibration;

		// Find first digit in the line
		for (char ch : line) {
			if (isdigit(ch)) {
				calibration += ch;
				break;
			}
		}

		// Find last digit in the line
		for (auto it = line.rbegin(); it != line.rend(); ++it) {
			if (isdigit(*it)) {
				calibration += *it;
				break;
			}
		}

		if (!calibration.empty()) {
			part1Calibrations.push_back(stoi(calibration));
		}
	}
}

// Extract and process part 2 calibrations
void CalibrationExtractor::extractPart2Calibrations() {
	regex digitFinder("(one|two|three|four|five|six|seven|eight|nine|zero|\\d)");

	for (const string& line : lines) {
		smatch matches;
		string part2Calibration;

		// Find all digit words or digits in the line
		string::const_iterator searchStart(line.cbegin());
		while (regex_search(searchStart, line.cend(), matches, digitFinder)) {
			part2Calibration += convertToDigit(matches[0]);
			searchStart = matches.suffix().first;
		}

		if (part2Calibration.size() >= 2) {
			// Ensure we have at least two digits
			part2Calibrations.push_back(stoi(part2Calibration));
		}
	}
}

// Process the calibrations
void CalibrationExtractor::process() {
	extractPart1Calibrations();
	extractPart2Calibrations();
}

// Get sum of part 1 calibrations
int CalibrationExtractor::getPart1Sum() const {
	int sum = 0;
	for (int num : part1Calibrations) {
		sum += num;
	}
	return sum;
}

// Get sum of part 2 calibrations
int CalibrationExtractor::getPart2Sum() const {
	int sum = 0;
	for (int num : part2Calibrations) {
		sum += num;
	}
	return sum;
}

int main() {
	CalibrationExtractor extractor("list.txt");
	extractor.process();

	cout << "Part 1 Sum: " << extractor.getPart1Sum() << endl;
	cout << "Part 2 Sum: " << extractor.getPart2Sum() << endl;

	return 0;
}
