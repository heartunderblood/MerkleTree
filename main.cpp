#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "openssl/sha.h"
using namespace std;

typedef unsigned int uint;
// ---- sha256摘要哈希 - --- //
void sha256(const
       std::string & srcStr, std::string & encodedStr, std::string & encodedHexStr)
{
// 调用sha256哈希
    unsigned char mdStr[33] = {0};
    SHA256((const unsigned char *)
                   srcStr.c_str(), srcStr.length(), mdStr);

// 哈希后的字符串
    encodedStr = std::string((const char *)
                                     mdStr);
// 哈希后的十六进制串32字节
    char buf[65] = {0};
    char tmp[3] = {0};
    for (int i = 0; i < 32; i++)
    {
        sprintf(tmp, "%02x", mdStr[i]);
        strcat(buf, tmp);
    }
    buf[64] = '\0'; // 后面都是0，从32字节截断
    encodedHexStr = std::string(buf);
}

class MerkleTreeNode
{
public:
    MerkleTreeNode *left;
    MerkleTreeNode *right;
    MerkleTreeNode *parent;
    uint level;
    string hash;

    MerkleTreeNode()
    {
        this ->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
        this->level = 0;

    }
};

typedef MerkleTreeNode MerkleTree;

void PrintMerkleTree(MerkleTree *mt)
{
    MerkleTree *p = mt;
    int i;

    if (p == nullptr)   return;     // empty tree

    if(p->left== nullptr && p->right==nullptr)
    {
//        for(i=0; i < high - p->level; i++)      cout << " ";
        cout << p->hash << endl;
    }
    else
    {
        PrintMerkleTree(p->left);
//        for(i=0; i< high- p->level; i++)        cout << " ";
        cout << p->hash;
        cout << endl;
        PrintMerkleTree(p->right);
    }

}

int count(MerkleTree *mt, int high)
{
    MerkleTree *p = mt;
    int i;

    if (p == nullptr)   return 0;     // empty tree

    if(p->left== nullptr && p->right==nullptr)
    {
        return 1;
    }
    else
    {
        int le = count(p->left, high);
        int ri = count(p->right, high);
        return le + ri + 1;
    }
}

MerkleTreeNode* CreateTree(string *Data)
{
    int i = 0, len = 0, l;
    while(Data[i].length() == 1)
    {
        i++;
        len++;
    }
    auto** Node = new MerkleTreeNode*[len];
    for(i = 0; i < len; i++)
    {
        Node[i] = new MerkleTreeNode;
    };
    string temp;
    for(i = 0; i < len; i++)
    {
        Node[i]->level = 0;
        sha256(Data[i], temp, Node[i]->hash);
    }
    l = len;
    string str;
    while(l > 1)
    {
        for(i = 0; 2 * i < l; i += 1)
        {
            if (l - 2 * i >= 2)
            {
                auto* tmp = new MerkleTreeNode;
                tmp->left = Node[2 * i];
                tmp->right = Node[2 * i + 1];
                Node[2 * i]->parent = tmp;
                Node[2 * i + 1]->parent = tmp;
                str = Node[2 * i]->hash + Node[2 * i + 1]->hash;
                sha256(str, temp, tmp->hash);
                Node[i] = tmp;
            }
            else if(l - 2 * i == 1)
            {
//                auto* tmp = new MerkleTreeNode;
//                tmp->left = Node[2 * i];
//                Node[2 * i]->parent = tmp;
//                tmp->left = Node[2 * i];
//                str = Node[2 * i]->hash + Node[2 * i]->hash;
//                sha256(str, temp, tmp->hash);
                Node[i] = Node[2 * i];
            }
        }
        if (l % 2 == 0) l = l/2;
        else l = int(l / 2) + 1;
    }
    return Node[0];
}

string* Gen(int len)
{
    int i = 0;
    string *mes = new string[100000];
//    string mes[100000];
    string alphabet = "0123456789zxcvbnmasdfghjklqwertyuiop";
    for (i; i < len; i++)
    {
        mes[i] = alphabet[rand() % 36];
    }
    return mes;
}

int main() {
    string DATA[] = {"1", "2", "3", "4", "5"};

//    cout << sizeof(DATA)/sizeof(DATA[0]);
//    cout << DATA[5];
    cout << "#-------------------------------------Create a MerkleTree-------------------------------------#" << endl ;
    MerkleTree* Tree =  CreateTree(DATA);
    PrintMerkleTree(Tree);
    /*这里跟李胜寒的长度不一样,虽然结果差不多,但是为什么?*/
    cout << "#---------------------------------------------------------------------------------------------#" << endl ;

    cout << "#-----------------------Create a MerkleTree with 100k leafnodes-------------------------------#" << endl;
    string* message;
    message = Gen(100000);
    Tree = CreateTree(message);
    PrintMerkleTree(Tree);
    cout << "#---------------------------------------------------------------------------------------------#" << endl;
//    cout << count(Tree, 3);
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
