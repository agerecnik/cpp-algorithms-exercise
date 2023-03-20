#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <numeric>

template <class T>
void print(const std::vector<T>& vec) {
	std::ostream_iterator<T> out(std::cout, ",");
	std::copy(cbegin(vec), cend(vec), out);
	std::cout << std::endl;
}

int main() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> idist(0, 1000);

	std::vector<int> vec;

	// 1. Fill a vector with 10 random integers between 0 and 1000.
	std::generate_n(std::back_inserter(vec), 10, [&mt, &idist]() {return idist(mt); });

	// 2. Print out the vector elements.
	print(vec);

	// 3. Find the maximum element in this vector.
	auto max_it = std::max_element(cbegin(vec), cend(vec));
	int max_element = *max_it;
	std::cout << "Max element: " << max_element << std::endl;

	// 4. Find the index of this maximum element.
	int index = std::distance(cbegin(vec), max_it);
	std::cout << "Index of the max element: " << index << std::endl;

	// 5. Sum the elements of the vector.
	int sum = std::accumulate(cbegin(vec), cend(vec), 0);
	std::cout << "Sum of the elements in the vector: " << sum << std::endl;

	// 6. Count the number of odd numbers in the vector.
	int count = std::count_if(cbegin(vec), cend(vec), [](int x) {return x % 2; });
	std::cout << "Number of odd numbers in the vector: " << count << std::endl;

	// 7. Normalize the vector (divide all elements by the largest element) and put the normalized elements into a vector of doubles,
	// without setting the size of the output vector first.
	std::vector<double> normalized_vec;
	std::transform(cbegin(vec), cend(vec), std::back_inserter(normalized_vec), [max_element](int x) {return (double)x / max_element; });
	std::cout << "Normalized vector: ";
	print(normalized_vec);

	// 8. Make a sorted copy of the vector. Without using a functor or a lambda, find the first element greater than 455 and the number of elements that are greater than 455.
	std::vector<int> sorted_vec;
	std::copy(cbegin(vec), cend(vec), std::back_inserter(sorted_vec));
	std::sort(begin(sorted_vec), end(sorted_vec));
	auto it = std::upper_bound(cbegin(sorted_vec), cend(sorted_vec), 455);
	int number_of_elements = std::distance(it, cend(sorted_vec));
	std::cout << "Sorted vector: ";
	print(sorted_vec);

	// 9. Copy all the odd numbers to a vector of doubles, without setting the size of the output vector first.
	std::vector<int> odd_vec;
	std::copy_if(cbegin(vec), cend(vec), std::back_inserter(odd_vec), [](int x) {return x % 2; });
	std::cout << "Odd vector: ";
	print(odd_vec);

	// 10. Sort the vector in descending order.
	std::sort(begin(vec), end(vec), [](int l, int r) {return r < l; });
	std::cout << "Sorted vector in descending order: ";
	print(vec);

	// 11. Randomly shuffle all but the first and the last element of the vector.
	std::shuffle(next(begin(vec)), prev(end(vec)), mt);
	std::cout << "Shuffled vector: ";
	print(vec);

	// 12. Remove all odd numbers from the vector.
	auto remove_it = std::remove_if(begin(vec), end(vec), [](int x) {return x % 2; });
	vec.erase(remove_it, end(vec));
	std::cout << "Removed odd numbers in the vector: ";
	print(vec);

	// 13. Write the remaining elements to a text file on a single line as a comma separated list, without a trailing comma.
	std::ofstream ofile("vec.txt");
	if (ofile && cbegin(vec) != cend(vec)) {
		std::ostream_iterator<int> out(ofile, ",");
		std::copy(cbegin(vec), prev(cend(vec)), out);
		std::ostream_iterator<int> out2(ofile);
		std::copy(prev(cend(vec)), cend(vec), out2);
	}

	auto normalize_word = [](const std::string& s) {
		std::string word{ "" };
		std::copy_if(cbegin(s), cend(s), std::back_inserter(word), [](char c) { return isalpha(c); });
		std::transform(begin(word), begin(word), begin(word), [](char c) { return tolower(c); });
		return word;
	};

	// 14. Read a file of words and display each word once. Ignore punctuation and capitalization.
	std::ifstream input_stream{ "words.txt" };
	if (input_stream) {
		std::istream_iterator<std::string> input_stream_it{ input_stream };
		std::istream_iterator<std::string> eof;
		std::vector<std::string> org_words{ input_stream_it, eof };

		std::vector<std::string> words;

		// Normalize the string and sort it.
		std::transform(cbegin(org_words), cend(org_words), std::back_inserter(words), normalize_word);
		std::sort(begin(words), end(words));

		// Remove duplicates.
		auto last_it = std::unique(begin(words), end(words));
		words.erase(last_it, end(words));

		std::cout << "The file words.txt contains the following words:" << std::endl;
		print(words);

		// 15. Count the total number of words in the file.
		std::cout << std::endl << "The file words.txt contains " << org_words.size() << " words" << std::endl;
	}

	// 16. Count the number of lines in the file.
	std::ifstream input_stream2("words.txt");

	if (input_stream2) {
		std::istreambuf_iterator<char> input_stream_it{ input_stream2 };
		std::istreambuf_iterator<char> eof;
		std::string characters{ input_stream_it, eof };

		auto nlines = std::count(cbegin(characters), cend(characters), '\n');
		std::cout << "The file words.txt contains " << nlines << " lines\n";

		// 17. Count the number of characters in the file.				
		std::cout << "The file words.txt contains " << characters.size() << " characters, including new line characters\n";
	}

	// 18. Read two files of words and display the words which are common to both files.
	std::ifstream input_stream3{ "words.txt" };
	std::ifstream input_stream4{ "words2.txt" };

	if (input_stream3 && input_stream4) {
		// Read word.txt and word2.txt into vectors.
		std::istream_iterator<std::string> input_stream_it{ input_stream3 };
		std::istream_iterator<std::string> eof;
		std::vector<std::string> org_words{ input_stream_it, eof };
		std::istream_iterator<std::string> input_stream_it2{ input_stream4 };
		std::vector<std::string> org_words2{ input_stream_it2, eof };

		std::vector<std::string> normalized_words, normalized_words2;

		// Normalize the words.txt vector.
		std::transform(cbegin(org_words), cend(org_words), std::back_inserter(normalized_words), normalize_word);
		std::sort(begin(normalized_words), end(normalized_words));
		auto last_it = std::unique(begin(normalized_words), end(normalized_words));
		normalized_words.erase(last_it, end(normalized_words));

		// Normalize the words2.txt vector.
		std::transform(cbegin(org_words2), cend(org_words2), std::back_inserter(normalized_words2), normalize_word);
		std::sort(begin(normalized_words2), end(normalized_words2));
		auto last_it2 = std::unique(begin(normalized_words2), end(normalized_words2));
		normalized_words2.erase(last_it2, end(normalized_words2));

		// Find the set intersection of the two vectors.
		std::cout << "The files words.txt and words2.txt have the following words in common:" << std::endl;
		std::ostream_iterator<std::string> out(std::cout, "\n");
		std::set_intersection(cbegin(normalized_words), cend(normalized_words), cbegin(normalized_words2), cend(normalized_words2), out);
	}

	// 19. Calculate the factorial of 6 (6 x 5 x 4 ... x 1).
	std::vector<int> numbers(6);
	std::iota(begin(numbers), end(numbers), 1);
	std::cout << "The factorial of 6 is " << std::accumulate(begin(numbers), end(numbers), 1, std::multiplies<int>());
}