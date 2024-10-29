#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>

// 二叉树节点类
template <typename T>
class BinTree {
public:
    T data;
    BinTree* left;
    BinTree* right;

    BinTree(T data) : data(data), left(nullptr), right(nullptr) {}
    ~BinTree() {
        delete left;
        delete right;
    }
};

// 位图类
class Bitmap {
private:
    unsigned char* M;
    int N, _sz;

protected:
    void init(int n) {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
        _sz = 0;
    }

public:
    Bitmap(int n = 8) { init(n); }
    ~Bitmap() { delete[] M; M = nullptr; _sz = 0; }

    int size() { return _sz; }
    void set(int k) {
        expand(k);
        _sz++;
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    void clear(int k) {
        expand(k);
        _sz--;
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }
    bool test(int k) {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }
    char* bits2string(int n) {
        expand(n - 1);
        char* s = new char[n + 1];
        s[n] = '\0';
        for (int i = 0; i < n; i++) s[i] = test(i) ? '1' : '0';
        return s;
    }

    void expand(int k) {
        if (k < 8 * N) return;
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }
};

// 哈夫曼树类
class HuffTree {
public:
    struct Node {
        char ch;
        int freq;
        Node* left;
        Node* right;
        Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
    };

    struct compare {
        bool operator()(Node* l, Node* r) {
            return l->freq > r->freq;
        }
    };

    Node* root;

    HuffTree(const std::unordered_map<char, int>& frequencies) {
        std::priority_queue<Node*, std::vector<Node*>, compare> minHeap;

        for (const auto& pair : frequencies) {
            minHeap.push(new Node(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            Node* left = minHeap.top(); minHeap.pop();
            Node* right = minHeap.top(); minHeap.pop();

            Node* combined = new Node('\0', left->freq + right->freq);
            combined->left = left;
            combined->right = right;
            minHeap.push(combined);
        }
        root = minHeap.top();
    }

    ~HuffTree() {
        delete root;
    }
};

// 哈夫曼编码类
class HuffmanCoding {
public:
    std::unordered_map<char, std::string> codes;

    void generateCodes(HuffTree::Node* root, const std::string& code) {
        if (!root) return;
        if (root->ch != '\0') codes[root->ch] = code;
        generateCodes(root->left, code + "0");
        generateCodes(root->right, code + "1");
    }

    HuffmanCoding(const std::unordered_map<char, int>& frequencies) {
        HuffTree tree(frequencies);
        generateCodes(tree.root, "");
    }

    std::string encode(const std::string& text) {
        std::string encoded;
        for (char ch : text) {
            encoded += codes[ch];
        }
        return encoded;
    }
};

// 主函数，执行编码
int main() {
    // 提取字符频率
    std::string text = R"(I am happy to join with you today in what will go down in history as the greatest demonstration for freedom in the history of our nation. Five score years ago, a great American, in whose symbolic shadow we stand today, signed the Emancipation Proclamation.)";
    
    // 统计字母频率
    std::unordered_map<char, int> frequencies;
    for (char ch : text) {
        if (isalpha(ch)) {
            frequencies[tolower(ch)]++; // 统计频率并转换为小写
        }
    }

    // 创建哈夫曼编码实例
    HuffmanCoding huff(frequencies);
    
    // 编码特定单词
    std::vector<std::string> words = {"dream", "freedom", "justice", "equality", "brotherhood", "hope"};
    for (const auto& word : words) {
        std::string encoded_word = huff.encode(word);
        std::cout << "Encoded '" << word << "': " << encoded_word << std::endl;
    }

    return 0;
}

