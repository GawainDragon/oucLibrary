
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

#include "datastore.h"

// ����һ��ͼ��
void InsertBook() {
    auto book = Datastore::Create<Datastore::Book>();
    strcpy(book->Name, "C++ Primer���İ�(��5��)");
    strcpy(book->Isbn, "9787121155352");
    strcpy(book->Author, "Stanley B. Lippman, Josee Lajoie, Barbara E. Moo");
    strcpy(book->Pulisher, "���ӹ�ҵ������");
    book->Total = 10;
    book->Remain = 10;
    Datastore::InsertOrUpdate(book);
    delete book;
}

// �޸�ͼ����Ϣ
void UpdateBook() {
    auto book = Datastore::Select<Datastore::Book>(0);
    book->Remain = 5;
    Datastore::InsertOrUpdate(book);
    delete book;
}

// ɾ��ͼ����Ϣ
void DeleteBook() {
    Datastore::Delete<Datastore::Book>(0);
}

// �ͷ��������
template <typename T>
void DestroyArray(T** array) {
    auto temp = array;
    while (*temp != NULL) {
        delete *temp;
        temp++;
    }
    delete array;
}

// ������������ͼ��
bool SearchBookCondition(const Datastore::Book* book) {
    return book->Remain > 0;
}
void SearchBook() {
    auto books = Datastore::Select<Datastore::Book>(SearchBookCondition);
    DestroyArray(books);
}

// ������������ͼ�� ��2��д��
void SearchBook2() {
    auto books = Datastore::Select<Datastore::Book>([](const Datastore::Book* book) {
        return book->Remain > 0;
    });
    DestroyArray(books);
}

// �޸��û�
void UpdateUser() {
    auto user = Datastore::Select<Datastore::User>(0);
    strcpy(user->Password, "123456");
    Datastore::InsertOrUpdate(user);
    delete user;
}

// ���ӽ��ļ�¼
void InsertRecord() {
    auto record = Datastore::Create<Datastore::Record>();
    record->BookIndex = 0;
    record->UserIndex = 1;
    record->Datetime = time(NULL);
    Datastore::InsertOrUpdate(record);
    delete record;
}

// ע���ʼ��
int main_() {
    Datastore::Init();
    InsertBook();
    UpdateBook();
    DeleteBook();
    SearchBook();
    SearchBook2();
    UpdateUser();
    InsertRecord();
    return 0;
}