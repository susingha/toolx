#include <iostream.h>


struct node
	{int data;
	node *l,*r;
	};


class bst
	{node *head;

	public:
	bst()
		{head=NULL;
		}


	void insert(int num)		//this is actually the create function
		{node *t=head;
 		if(t==NULL)
			{head=new node;
			head->l=head->r=NULL;
			head->data=num;
			}
		else
			{insert(num,t);
			}
		}
	void insert(int num,node *t)
		{if(num<=t->data)
			if(t->l==NULL)
				{t->l=new node;
				t=t->l;
				t->l=t->r=NULL;
				t->data=num;
				}
			else
				insert(num,t->l);
		else
			if(t->r==NULL)
				{t->r=new node;
				t=t->r;
				t->l=t->r=NULL;
				t->data=num;
				}
			else
				insert(num,t->r);
		}


	bool search(int);
	bool search(int,node*);
	};


int main()
	{bst t1;
	int data,choice;
	char f;
	do
		{cout<<"\n1.Insert data";
		cout<<"\n2.Search data";
		cout<<"\n3.Exit";
		cout<<"\n\nEnter choice : ";
		cin>>choice;

		switch(choice)
			{case 1:
				cout<<"\nEnter data : ";
				cin>>data;
				t1.insert(data);
				break;
			case 2:
				cout<<"Enter search velue : ";
				cin>>data;
				if(t1.search(data)==true)
					cout<<"\n"<<data<<" exists.";
				else
					cout<<"\n"<<data<<" does NOT exist.";
				break;
			case 3:
				exit(0);
				break;
			default:
				cout<<"\nInvalid input!!!";
			}

		cout<<"\nDo you want to continue? ";
		cin>>f;
		}while(f!='n'&&f!='N');
	return 0;
	}


bool bst::search(int num)
	{bool exists=false;
	if(head==NULL)
		cout<<"\nNo Tree available";
	else
		{node *t=head;
		if(num==t->data)
			exists=true;
		else if(num<t->data)
			exists=search(num,t->l);
		else
			exists=search(num,t->r);
		}
	return exists;
	}
bool bst::search(int num,node *t)
	{bool exists=false;
	if(t!=NULL)
		if(num==t->data)
			exists=true;
		else if(num<t->data)
			exists=search(num,t->l);
		else
			exists=search(num,t->r);
	return exists;
	}
