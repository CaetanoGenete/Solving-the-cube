#pragma once

#include <iostream>
#include <fstream>
#include <random>

#define BINARY_SEARCH_NOT_FOUND -1

namespace utils {

	class String;

	template<class T>
	inline void swap(T& lhs, T& rhs) {
		T temp = lhs;
		lhs = rhs;
		rhs = temp;
	}



	template<class T>
	int binarySearch(const T* data, const T searchee, int length) {
		int start = 0;
		int end = length - 1;

		while (start <= end) {
			unsigned int midPoint = (start + end) / 2;

			T searching = data[midPoint];

			if (searching == searchee)
				return midPoint;
			else if (searchee < searching)
				end = midPoint - 1;
			else if (searchee > searching)
				start = midPoint + 1;
		}

		return -1;
	}

	template<class T>
	class ArrayList {
	private:

		T * data;

		unsigned int allocatedSize;
		unsigned int noElements;

		void Swap(ArrayList<T>& arrayList) {
			swap(this->data, arrayList.data);
			swap(this->allocatedSize, arrayList.allocatedSize);
			swap(this->noElements, arrayList.noElements);
		}

	public:

		ArrayList() : data(nullptr), allocatedSize(0), noElements(0) {}

		ArrayList(unsigned int allocatedSize) : allocatedSize(allocatedSize), noElements(0) {
			data = new T[allocatedSize];
		}

		ArrayList(const std::initializer_list<T>& list) : ArrayList(list.size()) {
			data = new T[list.size()];

			for (T element : list)
				data[noElements++] = element;
		}

		ArrayList(const T* array, unsigned int count) : ArrayList(count + 1) {
			for (unsigned int i = 0; i < count; i++)
				data[noElements++] = array[i];
		}

		ArrayList(const ArrayList<T>& copy) : ArrayList(copy.allocatedSize) {
			noElements = copy.noElements;

			for (unsigned int i = 0; i < noElements; i++)
				data[i] = copy.data[i];
		}

		ArrayList(ArrayList<T>&& move) : ArrayList() {
			Swap(move);
		}

		~ArrayList() {
			delete[] data;
		}

		ArrayList<T>& operator=(const ArrayList<T>& copy) {
			allocatedSize = copy.allocatedSize;
			noElements = copy.noElements;

			delete[] data;

			data = new T[allocatedSize];

			for (unsigned int i = 0; i < noElements; i++)
				data[i] = copy.data[i];

			return *this;
		}

		ArrayList<T>& operator=(ArrayList<T>&& move) {
			delete[] data;

			data = nullptr;
			allocatedSize = 0;
			noElements = 0;

			Swap(move);

			return *this;
		}

		T& operator[](unsigned int index) const {
			return data[index];
		}

		T& get(unsigned int index) const {
			if (index < 0 || index >= noElements)
				throw std::out_of_range("index is out of range");
			else
				return data[index];
		}

		bool ensureCapacity(unsigned int space) {
			if (space > allocatedSize) {

				allocatedSize = space;

				T* newData = new T[space];

				for (unsigned int i = 0; i < noElements; i++)
					newData[i] = data[i];

				delete[] data;
				data = newData;

				return true;
			}
			return false;
		}

		ArrayList<T>& add(const T item) {
			if (noElements >= allocatedSize)
				ensureCapacity(allocatedSize + 1);

			data[noElements++] = item;

			return *this;
		}

		ArrayList<T>& add(const ArrayList<T>& array) {
			ensureCapacity(noElements + array.noElements);

			for (unsigned int i = 0; i < array.noElements; i++) add(array[i]);

			return *this;
		}

		void insert(const T item, unsigned int pos) {
			if (pos < 0 || pos >= allocatedSize)
				throw std::out_of_range("pos is out of range");

			if (pos > noElements)
				pos = noElements;

			if (noElements >= allocatedSize)
				allocatedSize++;
			else if (pos == noElements) {
				data[pos] = item;
				return;
			}

			T* newData = new T[allocatedSize];

			for (unsigned int i = 0; i < pos; i++)
				newData[i] = data[i];

			newData[pos] = item;

			for (unsigned int i = pos, ctr = pos + 1; i < noElements; i++, ctr++) {
				newData[ctr] = data[i];
			}

			delete[] data;
			data = newData;
			noElements++;
		}

		T pop() {
			if (noElements == 0)
				throw std::underflow_error("ArrayList is empty");

			return data[--noElements];
		}

		T remove(unsigned int index) {
			if (index < 0 || index >= noElements)
				throw std::out_of_range("index is out of bounds");

			if (noElements == 0)
				throw std::underflow_error("ArrayList is empty");

			noElements--;

			T item = data[index];

			for (unsigned int i = index; i < noElements; i++)
				data[i] = data[i + 1];

			return item;
		}

		void clear() {
			noElements = 0;
		}

		T* begin() {
			if (noElements > 0)
				return &data[0];
			else
				return nullptr;
		}

		T* end() {
			if (noElements > 0)
				return &data[noElements];
			else
				return nullptr;
		}

		ArrayList<T>& flip() {

			int start_ptr = 0;
			int end_ptr = noElements - 1;
			while(start_ptr++ < end_ptr--) {
				T temp = data[start_ptr];
				data[start_ptr] = data[end_ptr];
				data[end_ptr] = temp;
			}

			return *this;
		}

		ArrayList<T>& trim() {
			T* newData = new T[allocatedSize = noElements];

			for (unsigned int i = 0; i < noElements; i++)
				newData[i] = data[i];

			delete[] data;
			data = newData;

			return this;
		}

		ArrayList<T>& shuffle() {
			for (unsigned int i = 0; i < noElements; i++) {
				unsigned int randElement = std::rand() % noElements;

				T temp = data[i];
				data[i] = data[randElement];
				data[randElement] = temp;
			}
		}

		inline T& getFirst() {
			return data[0];
		}

		inline T& getLast() {
			return data[noElements - 1];
		}

		inline unsigned int getSize() const {
			return noElements;
		}

		inline const T* getData() const {
			return data;
		}

	};

	template<class T>
	class Queue {
	private:
		ArrayList<T> data;

		int start, end;

	public:

		Queue(): start(0), end(-1) {}
		Queue(unsigned int size) : Queue() {
			data = ArrayList<T>(size);
		}

		void push(const T item) {
			data.add(item);

			end++;
		}

		T pop() {
			if (end >= start)
				return data[start++];
			else
				std::cout << "ERROR" << std::endl;
		}
	};

	template<class T>
	class CircularQueue {
	protected:
		T * data;

		unsigned int start;
		unsigned int end;

		unsigned int allocatedSize;
		unsigned int noElements;

		inline void Swap(CircularQueue<T>& queue) {
			swap(data, queue.data);
			swap(start, queue.start);
			swap(allocatedSize, queue.allocatedSize);
			swap(noElements, queue.noElements);
		}

		inline unsigned int next(unsigned int number) {
			number++;

			if (number >= allocatedSize)
				number = 0;

			return number;
		}

	public:

		CircularQueue() : data(nullptr), start(0), end(0), allocatedSize(0), noElements(0) {

		}

		CircularQueue(const unsigned int allocatedSize) : data(nullptr), start(0), end(0), noElements(0), allocatedSize(allocatedSize) {
			data = new T[allocatedSize];
		}

		CircularQueue(const std::initializer_list<T>& list, const unsigned int allocatedSize) : CircularQueue(allocatedSize) {
			for (T element : list)
				data[noElements++] = element;

			end = noElements - 1;
		}

		CircularQueue(const CircularQueue<T>& copy) : CircularQueue(copy.allocatedSize) {
			start = copy.start;
			end = copy.end;

			noElements = copy.noElements;

			for (unsigned int i = 0, j = start; i < noElements; i++, j = next(j))
				data[j] = copy.data[j];
		}

		CircularQueue(CircularQueue<T>&& move) : CircularQueue() {
			Swap(move);
		}

		~CircularQueue() {
			delete[] data;
		}

		CircularQueue<T>& operator=(const CircularQueue<T>& copy) {
			start = copy.start;
			end = copy.end;

			allocatedSize = copy.allocatedSize;
			noElements = copy.noElements;

			delete[] data;

			data = new T[allocatedSize];

			for (unsigned int i = 0, j = start; i < noElements; i++, j = next(j))
				data[j] = copy.data[j];
		}

		CircularQueue<T>& operator=(CircularQueue<T>&& move) {
			start = 0;
			end = 0;

			allocatedSize = 0;
			noElements = 0;

			delete[] data;
			data = nullptr;

			Swap(move);
		}

		CircularQueue<T>& add(const T data) {
			if (isFull())
				throw std::overflow_error("Circular queue is full");
			else {
				this->data[end] = data;

				end = next(end);
				noElements++;
			}

			return *this;
		}

		const T& pop() {
			if (isEmpty())
				throw std::underflow_error("Circular queue is empty");

			const T* result = data + start;

			start = next(start);
			noElements--;

			return *result;
		}

		const T& peek() const {
			if (!noElements)
				throw std::underflow_error("Circular Queue is empty");

			return data[start];
		}

		inline bool isFull() const {
			return noElements >= allocatedSize;
		}

		inline bool isEmpty() const {
			return noElements == 0;
		}

		inline unsigned int getLength() const {
			return noElements;
		}

	};

	template<class T>
	struct PriorityQueueElement {
	public:
		const static int MIN_PRIORITY = INT_MAX - 1;

		T data;
		int priority = INT_MAX;
	};

	template<class T>
	class PriorityQueue : public CircularQueue<PriorityQueueElement<T>> {

	private:
		using CircularQueue<PriorityQueueElement<T>>::data;

		using CircularQueue<PriorityQueueElement<T>>::start;
		using CircularQueue<PriorityQueueElement<T>>::end;

		using CircularQueue<PriorityQueueElement<T>>::allocatedSize;
		using CircularQueue<PriorityQueueElement<T>>::noElements;

		using CircularQueue<PriorityQueueElement<T>>::next;

	public:

		using CircularQueue<PriorityQueueElement<T>>::CircularQueue;

		PriorityQueue<T>& add(const T data, int priority) {
			if (CircularQueue<PriorityQueueElement<T>>::isFull())
				throw std::overflow_error("Priority queue is full");
			else {
				unsigned int insertAt = start;
				for (unsigned int i = 0; i < noElements; i++, insertAt = next(insertAt))
					if (this->data[insertAt].priority > priority)
						break;

				unsigned int current = end;
				while (current != insertAt) {
					unsigned int next = current;

					if (current == 0)
						current = allocatedSize - 1;
					else
						current--;

					this->data[next] = this->data[current];
				}

				this->data[insertAt] = { data, priority };

				end = next(end);
				noElements++;
			}

			return *this;
		}

		PriorityQueue<T>& add(const T data) {
			CircularQueue<PriorityQueueElement<T>>::add({ data, PriorityQueueElement<T>::MIN_PRIORITY });

			return *this;
		}

		const T& pop() {
			return CircularQueue<PriorityQueueElement<T>>::pop().data;
		}

		const T& peek() const {
			return CircularQueue<PriorityQueueElement<T>>::peek().data;
		}
	};

	template<class T>
	struct DictionaryElement {
	public:
		T data;
		String key;
	};

	template<class T>
	bool operator==(DictionaryElement<T> lhs, DictionaryElement<T> rhs) {
		return lhs.key == rhs.key;
	}

	template<class T>
	bool operator<(DictionaryElement<T> lhs, DictionaryElement<T> rhs) {
		return lhs.key < rhs.key;
	}

	template<class T>
	bool operator>(DictionaryElement<T> lhs, DictionaryElement<T> rhs) {
		return lhs.key > rhs.key;
	}

	template<class T>
	class Dictionary {
	private:
		ArrayList<DictionaryElement<T>> elements;

		int cachedLocation;
		String cachedKey;

	public:

		Dictionary() : cachedLocation(0) {
		}

		/*

		Dictionary(std::initializer_list<DictionaryElement<T>>& list) : Dictionary() {
			//elements = ArrayList<DictionaryElement<T>>(list);
		}

		*/

		Dictionary& add(const T item, const String key) {
			unsigned int i = 0;
			for (; i < elements.getSize(); i++)
				if (elements[i].key > key) break;

			elements.insert({ item, key }, i);

			return *this;
		}

		bool keyExists(const String& key) {
			int location = binarySearch(elements.getData(), { NULL, key }, elements.getSize());

			if (location != -1) {
				cachedLocation = location;
				cachedKey = key;

				return true;
			}
			else
				return false;
		}

		T& getItem(const String& key) {
			if (key == cachedKey)
				return elements[cachedLocation].data;

			return elements[binarySearch(elements.getData(), { NULL, key }, elements.getSize())].data;
		}

		T& remove(const String& key) {
			if (key == cachedKey)
				return elements.remove(cachedLocation);
			else
				return elements.remove(binarySearch(elements.getData(), { NULL, key }, elements.getSize()));
		}
	};

	class String {
	private:
		char* characters;

		unsigned int characterCount = 0;

		void Swap(String& string);

	public:

		String();

		String(const char character);
		String(int number);

		String(const char* characters);

		String(const char* characters, unsigned int characterCount);

		String(std::ifstream& stream);

		String(const String& copyString);

		String(String&& moveString) noexcept;

		~String() noexcept;

		String& operator=(const String& copyString);
		String& operator=(String&& moveString);

		String& remove(const String& string, bool all = false);

		String& operator+=(const String& string);
		String& operator-=(const String& string);

		String& operator*=(double multiplier);
		unsigned int operator/(const String& string) const;

		char operator[](unsigned int index);

		const char at(unsigned int index) const;

		String& replace(const String& toReplace, const String& replacer);

		String& flip();

		String upper() const;
		String lower() const;

		unsigned int length() const;

		const char* getData() const;

	};

	std::ostream& operator<<(std::ostream& stream, const String& string);

	bool operator==(const char* lhs, const String& rhs);
	bool operator==(const String& lhs, const String& rhs);

	bool operator!=(const char* lhs, const String& rhs);
	bool operator!=(const String& lhs, const String& rhs);

	bool operator>=(const String& lhs, const String& rhs);
	bool operator>=(const char* lhs, const String& rhs);
	bool operator<=(const String& lhs, const String& rhs);
	bool operator>=(const char* lhs, const String& rhs);

	bool operator> (const String& lhs, const String& rhs);
	bool operator> (const char* lhs, const String& rhs);
	bool operator< (const String& lhs, const String& rhs);
	bool operator< (const char* lhs, const String& rhs);

	String operator+(const char* lhs, const String& rhs);
	String operator+(const String& lhs, const String& rhs);

	String operator-(const char* lhs, const String& rhs);
	String operator-(const String& lhs, const String& rhs);

	String operator*(const String& lhs, double amount);

	String subString(const String& string, unsigned int start, unsigned end);

	String toString(int number);

	class BitStream {
	private:

		const unsigned char* stream;

		unsigned int remainingBits;
		unsigned int currentByte;

	public:

		BitStream();

		BitStream(const unsigned char* stream, unsigned int currentByte = 0);

		unsigned char readBit();
		unsigned int readUInt(unsigned int bits);

		unsigned char nextUByte();
		char nextByte();

		unsigned int nextUInt();
		int nextInt();

		String nextString(unsigned int length);
		void toBoundary();

		unsigned int getCurrentByte();
		void toByte(unsigned int byte);
	};

	template<unsigned int rows, unsigned int columns, class T>
	struct Matrix {

	protected:

		T elements[rows][columns];

	public:

		Matrix() {
			for (unsigned int x = 0; x < rows; x++)
				for (unsigned int y = 0; y < columns; y++)
					elements[x][y] = 0;
		}

		Matrix(const T elements[rows][columns]) {
			for (unsigned int x = 0; x < rows; x++)
				for (unsigned int y = 0; y < columns; y++)
					this->elements[x][y] = elements[x][y];
		}

		Matrix(const Matrix& copy) {
			for (unsigned int x = 0; x < rows; x++)
				for (unsigned int y = 0; y < columns; y++)
					elements[x][y] = copy.elements[x][y];
		}

		Matrix& operator=(const Matrix& copy) {
			for (unsigned int x = 0; x < rows; x++)
				for (unsigned int y = 0; y < columns; y++)
					elements[x][y] = copy.elements[x][y];

			return *this;
		}

		inline T* operator[](unsigned int row) {
			return elements[row];
		}

		inline const T* operator[](unsigned int row) const {
			return elements[row];
		}

	};

	template<unsigned int rows, unsigned int columns, unsigned int columns2, class T>
	Matrix<rows, columns2, T> operator*(const Matrix<rows, columns, T>& operand, const Matrix<columns, columns2, T>& operant) {
		Matrix<rows, columns2, T> result;

		for (int i = 0; i < rows; i++)
			for (int k = 0; k < columns; k++)
				for (int j = 0; j < columns2; j++)
					result[i][j] += operand[i][k] * operant[k][j];

		return result;
	}


	template<unsigned int rows, unsigned int columns, class T>
	bool operator==(const Matrix<rows, columns, T>& mat1, const Matrix<rows, columns, T>& mat2) {
		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				if (mat1[x][y] != mat2[x][y]) return false;

		return true;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<rows, columns, T>& operator+=(Matrix<rows, columns, T>& operant, const Matrix<rows, columns, T>& operand) {
		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				operant[x][y] += operand[x][y];

		return operant;
	}


	template<unsigned int rows, unsigned int columns, class T>
	Matrix<rows, columns, T>& operator-=(Matrix<rows, columns, T>& operant, const Matrix<rows, columns, T>& operand) {
		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				operant[x][y] -= operand[x][y];

		return operant;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<rows, columns, T> operator+(Matrix<rows, columns, T> operant, const Matrix<rows, columns, T>& operand) {
		return operant += operand;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<rows, columns, T> operator-(Matrix<rows, columns, T> operant, const Matrix<rows, columns, T>& operand) {
		return operant -= operand;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<rows, columns, T> operator-(Matrix<rows, columns, T> operant) {
		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				operant[x][y] = -operant[x][y];

		return operant;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<columns, rows, T> transpose(const Matrix<rows, columns, T>& matrix) {
		Matrix<columns, rows, T> result;

		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				result[y][x] = matrix[x][y];

		return result;
	}

	template<unsigned int rows, unsigned int columns, class T>
	Matrix<columns, rows, T>& vectoriseFunction(Matrix<columns, rows, T>& matrix, T(*function)(T&)) {
		Matrix<columns, rows, T> result;

		for (unsigned int x = 0; x < rows; x++)
			for (unsigned int y = 0; y < columns; y++)
				result[x][y] = function(matrix[x][y]);

		return result;
	}

	template<unsigned int rows, unsigned int columns, class T>
	std::ostream& operator<<(std::ostream& stream, const Matrix<rows, columns, T>& matrix) {
		for (unsigned int y = 0; y < rows; y++) {
			stream << "[";

			for (unsigned int x = 0; x < columns - 1; x++)
				stream << matrix[y][x] << ", ";

			stream << matrix[y][columns - 1] << "]";
			stream << std::endl;
		}

		return stream;
	}

	template<unsigned int size, class T>
	struct Vector : public Matrix<size, 1, T> {

		using Matrix<size, 1, T>::elements;

		using Matrix<size, 1, T>::Matrix;

		Vector() : Matrix<size, 1, T>() {}

		Vector(const std::initializer_list<T>& list) {
			for (unsigned int i = 0; i < size; i++)
				elements[i][0] = *(list.begin() + i);
		}

		inline T& operator[](unsigned int index) {
			return elements[index][0];
		}

	};

	template<unsigned int size, class T>
	Vector<size, T>& operator*=(Vector<size, T>& operant, const Vector<size, T>& operand) {
		for (unsigned int i = 0; i < size; i++)
			operant[i] *= operand[i];

		return operant;
	}

	template<unsigned int size, class T>
	Vector<size, T>& operator*(Vector<size, T> operant, const Vector<size, T>& operand) {
		return operant *= operand;
	}

	template<unsigned int size, class T>
	T dot(const Vector<size, T>& operant, const Vector<size, T>& operand) {
		T result = 0;

		for (unsigned int i = 0; i < size; i++)
			result += operand[i] * operant[i];

		return result;
	}

}
