#include "booker.h"
#include "datastore.h"

namespace Booker{
///�ڲ�ʹ��
	//Isbn��������
	string Temp, AnotherTemp;
	int BookIndex, UserIndex;
	bool SearchBookCondition(const Datastore::Book* book)
	{
		return !strcmp(book->Isbn, &Temp[0]);
	}
	
	//User��������
	bool SearchUserCondition(const Datastore::User* user)
	{
		return !strcmp(user->Name, &Temp[0]);
	}
	
	//ģ������ͼ��
	bool AnotherSearchBookCondition(const Datastore::Book* book)
	{
		if (DistanceBetweenThem(Temp, book->Isbn) < Temp.length() / 3.0)
		{
			return true;
		}
		else if (DistanceBetweenThem(Temp, book->Name) < Temp.length() / 3.0)
		{
			return true;
		}
		else if (DistanceBetweenThem(Temp, book->Author) < Temp.length() / 3.0)
		{
			return true;
		}
		else if (DistanceBetweenThem(Temp, book->Publisher) < Temp.length() / 3.0)
		{
			return true;
		}

		return false;
	}

	//�༭����,��������
	int DistanceBetweenThem(string a, string b)
	{
		int lena = a.length(), lenb = b.length(), i = 0;
	
		int **dp = new int*[lena + 1];
	
		for (i = 0; i <= lena; i++)
		{
			dp[i] = new int[lenb + 1];
		}
	
		for (i = 0; i <= lenb; i++)
		{
			dp[0][i] = i;
		}
	
		for (i = 0; i <= lena; i++)
		{
			dp[i][0] = i;
		}
	
		for (i = 1; i <= lena; i++)
		{
			for (int j = 1; j <= lenb; j++)
			{
				int cost = a[i - 1] == b[j - 1] ? 0 : 1;
				dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1) < dp[i - 1][j - 1] + cost ? 
					min(dp[i - 1][j] + 1, dp[i][j - 1] + 1) : dp[i - 1][j - 1] + cost;
			}
		}

		int result = dp[lena][lenb];

		for (i = 0; i <= lena; i++)
		{
			delete [] dp[i];
		}
	
		delete [] dp;
	
		return result;
	}
	
	//Accout������¼
	bool SearchRecordCondition(const Datastore::Record* record)
	{
		return !(record->UserIndex - UserIndex);
	}

	//Account&Isbn������¼
	bool AnotherSearchRecordCondition(const Datastore::Record* record)
	{
		return (((!(record->UserIndex - UserIndex)) & (!(record->BookIndex - BookIndex))) & (!record->IsReturned));
	}

	//Account&Isbn���Ҽ�¼
	Datastore::Record** IsbnFindRecord(string Account, string Isbn)
	{
		Temp = Account;
		Datastore::User **user = Datastore::Selects<Datastore::User>(SearchUserCondition);
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);
		if (user[0] != NULL && book[0] != NULL)
		{
			UserIndex = user[0]->Index;
			BookIndex = book[0]->Index;
			Datastore::Record **record = Datastore::Selects<Datastore::Record>(AnotherSearchRecordCondition);
			delete [] user;
			delete [] book;
			return record;
		}
		else
		{
			return NULL;
		}
	}

///�ⲿInterFace
	//����ͼ��
	bool AddBook(string Isbn, string Name, string Author, string Publisher, int Num)
	{
		if (Isbn.length() == 13 && Name.length() > 0 && Name.length() < 100 && Author.length() > 0 && Author.length() < 100 && Publisher.length() > 0 && Publisher.length() < 100 && Num > 0)
		{
			for (int i = 0; i < 13; i++)
			{
				if (!(Isbn[i] >= '0' && Isbn[i] <= '9' || Isbn[i] == 'X'))
				{
					return false;
				}
			}

			Temp = Isbn;
			Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

			if (book[0] != NULL)
			{
				book[0]->Total += Num;
				book[0]->Remain += Num;
				Datastore::InsertOrUpdate(book[0]);
				return true;
			}
			
			Datastore::Book *books = Datastore::Create<Datastore::Book>();
			strcpy(books->Name, &Name[0]);
			strcpy(books->Isbn, &Isbn[0]);
			strcpy(books->Author, &Author[0]);
			strcpy(books->Publisher, &Publisher[0]);
			books->Total = Num;
			books->Remain = Num;
			Datastore::InsertOrUpdate(books);
			delete book;

			return true;
		}
		else
		{
			return false;
		}
	}
	
	//ɾ��ͼ��
	bool DeleteBook(string Isbn, int Num)
	{
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (book[0] != NULL && book[0]->Remain >= Num)
		{
			book[0]->Remain -= Num;
			book[0]->Total -= Num;
			Datastore::InsertOrUpdate(book[0]);
			delete [] book;
			return true;
		}

		return false;
	}

	//����Isbn
	bool ChangeBookIsbn(string Isbn, string NewIsbn)
	{
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);
		Temp = NewIsbn;
		Datastore::Book **newbook = Datastore::Selects<Datastore::Book>(SearchBookCondition);
		if (book[0] != NULL && newbook[0] == NULL)
		{
			strcpy(book[0]->Isbn, &Isbn[0]);
			Datastore::InsertOrUpdate(book[0]);
			delete [] book;
			return true;
		}

		return false;
	}
	
	//����Name
	bool ChangeBookName(string Isbn, string Name)
	{
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (book[0] != NULL)
		{
			strcpy(book[0]->Name, &Name[0]);
			Datastore::InsertOrUpdate(book[0]);
			delete [] book;
			return true;
		}

		return false;
	}
	
	//����Author
	bool ChangeBookAuthor(string Isbn, string Author)
	{
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (book[0] != NULL)
		{
			strcpy(book[0]->Publisher, &Author[0]);
			Datastore::InsertOrUpdate(book[0]);
			delete [] book;
			return true;
		}

		return false;
	}
	
	//����Publisher
	bool ChangeBookPublisher(string Isbn, string Publisher)
	{
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (book[0] != NULL)
		{
			strcpy(book[0]->Publisher, &Publisher[0]);
			Datastore::InsertOrUpdate(book[0]);
			delete [] book;
			return true;
		}

		return false;
	}

	//Isbn����Book
	Datastore::Book** IsbnFindBook(string Isbn)
	{
		Temp = Isbn;

		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (book[0] == NULL)	//û�ҵ�
		{
			return NULL;
		}

		return book;
	}

	//��������ͼ��
	Datastore::Book** AnythingFindBook(string Anything)
	{
		Temp = Anything;

		Datastore::Book **book = Datastore::Selects<Datastore::Book>(AnotherSearchBookCondition);

		if (book[0] == NULL)	//û�ҵ�
		{
			return NULL;
		}

		return book;
	}
	
	//Account���Ҽ�¼
	Datastore::Record** AccountFindRecord(string Account)
	{
		Temp = Account;
		Datastore::User **user = Datastore::Selects<Datastore::User>(SearchUserCondition);
		
		if (user[0] != NULL)
		{
			UserIndex = user[0]->Index;
			Datastore::Record **record = Datastore::Selects<Datastore::Record>(AnotherSearchRecordCondition);
			delete [] user;
			return record;
		}
		else
		{
			return NULL;
		}
	}

	//����ͼ��
	bool BrowseBook(string Account, string Isbn)
	{
		Temp = Account;
		Datastore::User **user = Datastore::Selects<Datastore::User>(SearchUserCondition);
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		if (user[0] != NULL && book[0] != NULL && book[0]->Remain > 0)
		{
			book[0]->Remain--;
			Datastore::InsertOrUpdate(book[0]);

			Datastore::Record *record = new Datastore::Record;
			record->UserIndex = user[0]->Index;
			record->BookIndex = book[0]->Index;
			record->Datetime = clock();
			record->IsReturned = false;
			Datastore::InsertOrUpdate(record);
			delete [] user;
			delete [] book;
			return true;
		}

		return false;
	}
	
	//�黹ͼ��
	int ReturnBook(string Account, string Isbn)
	{
		Temp = Account;
		Datastore::User **user = Datastore::Selects<Datastore::User>(SearchUserCondition);
		Temp = Isbn;
		Datastore::Book **book = Datastore::Selects<Datastore::Book>(SearchBookCondition);

		UserIndex = user[0]->Index;
		BookIndex = book[0]->Index;

		Datastore::Record **record = IsbnFindRecord(Account, Isbn);

		delete [] user;
		delete [] book;

		for (int i = 0; record[i] != NULL; i++)
		{
			if (record[i]->IsReturned = false)
			{
				record[i]->IsReturned = true;
				Datastore::InsertOrUpdate(record[i]);

				if ((double)(clock() - record[i]->Datetime) / CLOCKS_PER_SEC > 30 * 86400)
				{
					return -1;
				}

				return 1;
			}
		}

		return 0;
	}
}