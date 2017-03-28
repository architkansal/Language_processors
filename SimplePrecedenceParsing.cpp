#include<bits/stdc++.h>
using namespace std;

set<char> ct; // always kept empty		
set<string> st;	 // always kept empty		
set<char> term;	// set of terminals	
set<char> non_term;	// set of non_terminals		
map<char,set<string> >prod;	// Productions for each non_terminal
set<char> symbols; // Set of all grammer sybmols
map<pair<char,char>, char>parsing_table;   // Parsing table -- indexed on (T, T) --> Relation b/w them( <, >, =) 
map<char, set<char> > first;	// first of each non_terminal
map<char, set<char> > derives;	// what each non-terminal can derive in A =>(+) Ygamma steps  of each non_terminal
map<char, set<char> > derives_back;	// what each non-terminal can derive in A =>(+) alphaX steps  of each non_terminal

char start_symbol;
string str;

void reverseStr(string &str)
{
    int n = str.length();
 
    // Swap character starting from two
    // corners
    for (int i=0; i<n/2; i++)
       swap(str[i], str[n-i-1]);
}

void first_calc()
{
	cout<<"\n Calculating first ... "<<endl;
	
	for(set<char>::iterator it = term.begin(); it!=term.end();it++)
	{
		// ERRRROOOOORRRRR MIGHT COME DUE TO EPSILON IE WHITE SPACE SEE THAAAAAAAAAAAAAAAAAAAAATTTTTTTTTTT
		first[*it] = ct;
		first[*it].insert(*it);
	}
	
	int flag=1;
	for(set<char>::iterator i = non_term.begin(); i!= non_term.end(); i++ )
	{
		char nt = *i;
		first[nt] = ct;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			if(s=="epsilon")
			{
				first[nt].insert(' '); // WHITE SPACE INDICATES EPSILON PRODUCTION
				continue;
			}
			flag=0;
			for(int x=0;x<s.size(); x++)
			{
				if(!isupper(s[x]))	// terminal
				{
					first[nt].insert(s[x]);
					break;
				}
				else	// NON-Terminal
				{
					first[nt].insert(s[x]);
					for(set<string>::iterator k = prod[s[x]].begin(); k!=prod[s[x]].end(); k++ )
					{
						if(*k=="epsilon") 
							flag=1;		
					}				
					if(flag==0)
					{
						break;
					}
				}			
			}
		}
	}
	flag=3;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			for(set<char>::iterator j=first[nt].begin(); j!=first[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					for(set<char>::iterator k=first[ch].begin(); k!=first[ch].end(); k++)
					{
						first[nt].insert(*k);
					}
				}
			}
		}
	}
	
	flag=2;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			for(set<char>::iterator j=first[nt].begin(); j!=first[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					first[nt].erase(ch);
					j--;
				}
			}
		}
	}

	//Printing set of first of each non_terminals
	cout<< "\n First(A) "<<endl;
	for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
	{
		char nt=*i;
		cout<<nt<<" --- ";
		// if(leading[nt])
		for(set<char>::iterator j=first[nt].begin(); j!=first[nt].end(); j++)
		{
			char ch = *j;
			if(ch==' ')
			{
				cout<<"epsilon"<<" ";
				continue;
			}
			cout<<ch<<" ";
		}
		cout<<" "<<endl;
	}
	
	
}

void derives_calc()
{
	cout<<"\n Calculating Derive(s) ... "<<endl;
	
	int flag=1;
	for(set<char>::iterator i = non_term.begin(); i!= non_term.end(); i++ )
	{
		char nt = *i;
		derives[nt] = ct;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			if(s=="epsilon")
			{
				continue;
				// Nothing can be derived, Table does not contain epsilon entries
			}
			flag=0;
			for(int x=0;x<s.size(); x++)
			{
				if(!isupper(s[x]))	// terminal
				{
					derives[nt].insert(s[x]);
					break;
				}
				else	// NON-Terminal
				{
					derives[nt].insert(s[x]);
					for(set<string>::iterator k = prod[s[x]].begin(); k!=prod[s[x]].end(); k++ )
					{
						if(*k=="epsilon") 		
						{
							flag=1;
							continue;
						}
						else
						{
							string sst = *k;
							derives[nt].insert(sst[0]); //can be terminal or non-terminal
						}
					}
					if(flag==0)
						break;		
				}			
			}
		}
	}
	
	flag=3;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			for(set<char>::iterator j=derives[nt].begin(); j!=derives[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					for(set<char>::iterator k=derives[ch].begin(); k!=derives[ch].end(); k++)
					{
						derives[nt].insert(*k);
					}
				}
			}
		}
	}

	//Printing set of derives of each non_terminals
	cout<< "\n Derives(A) "<<endl;
	for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
	{
		char nt=*i;
		cout<<nt<<" --- ";
		// if(leading[nt])
		for(set<char>::iterator j=derives[nt].begin(); j!=derives[nt].end(); j++)
		{
			char ch = *j;
			if(ch==' ')
			{
				cout<<"epsilon"<<" ";
				continue;
			}
			cout<<ch<<" ";
		}
		cout<<" "<<endl;
	}
	
}


void derives_backward_calc()
{
	cout<<"\n Calculating Derives_Backward(s) ... "<<endl;
	
	int flag=1;
	for(set<char>::iterator i = non_term.begin(); i!= non_term.end(); i++ )
	{
		char nt = *i;
		derives_back[nt] = ct;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			if(s=="epsilon")
			{
				continue;
				// Nothing can be derived, Table does not conatin epsilon
			}
			flag=0;
			for(int x=s.size()-1;x>=0; x--)
			{
				if(!isupper(s[x]))	// terminal
				{
					derives_back[nt].insert(s[x]);
					break;
				}
				else	// NON-Terminal
				{
					derives_back[nt].insert(s[x]);
					for(set<string>::iterator k = prod[s[x]].begin(); k!=prod[s[x]].end(); k++ )
					{
						if(*k=="epsilon") 		
						{
							flag=1;
							continue;
						}
						else
						{
							string sst = *k;
							derives_back[nt].insert(sst[sst.size()-1]); //can be terminal or non-terminal
						}
					}
					if(flag==0)
						break;				
				}			
			}
		}
	}
	
	flag=3;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			for(set<char>::iterator j=derives_back[nt].begin(); j!=derives_back[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					for(set<char>::iterator k=derives_back[ch].begin(); k!=derives_back[ch].end(); k++)
					{
						derives_back[nt].insert(*k);
					}
				}
			}
		}
	}

	//Printing set of derives_back of each non_terminals
	cout<< "\n Derives_Back(A) "<<endl;
	for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
	{
		char nt=*i;
		cout<<nt<<" --- ";
		// if(leading[nt])
		for(set<char>::iterator j=derives_back[nt].begin(); j!=derives_back[nt].end(); j++)
		{
			char ch = *j;
			if(ch==' ')
			{
				cout<<"epsilon"<<" ";
				continue;
			}
			cout<<ch<<" ";
		}
		cout<<" "<<endl;
	}
	
}



void resolve_conflict()
{
	return;
}

void table_calc()
{
	pair<char,char>pr;
	for(set<char>::iterator it = non_term.begin(); it!= non_term.end(); it++ )
	{
		char nt = *it;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			int i=0;
			for(;i<s.size()-1;i++)
			{
				// Xi and Xi+1 are the grammer symbols
				pr = make_pair(s[i],s[i+1]);
				if(parsing_table.find(pr)==parsing_table.end())
				{
					parsing_table[pr] = '=';
				}
				else
				{
					cout<<"\n Following conflict occurred while constructing table : "<<endl;
					cout<<" for ("<<s[i]<<","<<s[i+1]<<") '=' as well as '"<<parsing_table[pr]<<"'"<<endl;
					cout<<s<<endl;
					resolve_conflict(); 
					cout<<"\n Exiting... ";
					exit(0);
				}	
				
				if(isupper(s[i+1]))
				{
					
					for(set<char>::iterator k=derives[s[i+1]].begin(); k!=derives[s[i+1]].end(); k++)
					{
						pr = make_pair(s[i],*k);
						if(parsing_table.find(pr)==parsing_table.end())
						{
							parsing_table[pr] = '<';
						}
						else
						{
							cout<<"\n Following conflict occurred while constructing table : "<<endl;
							cout<<" for ("<<s[i]<<","<<*k<<") '<' as well as '"<<parsing_table[pr]<<"'"<<endl;
							cout<<s<<endl;
							resolve_conflict(); 
							cout<<"\n Exiting... ";
							exit(0);
						}	
					}
				}
				
				if(isupper(s[i]))
				{
					for(set<char>::iterator k=first[s[i+1]].begin(); k!=first[s[i+1]].end(); k++)
					{
						for(set<char>::iterator x=derives_back[s[i]].begin(); x!=derives_back[s[i]].end(); x++)
						{
							
							pr = make_pair(*x,*k);
							if(parsing_table.find(pr)==parsing_table.end())
							{
								parsing_table[pr] = '>';
							}
							else
							{
								cout<<"\n Following conflict occurred while constructing table : "<<endl;
								cout<<" for ("<<*x<<","<<*k<<") '>' as well as '"<<parsing_table[pr]<<"'"<<endl;
								cout<<s<<endl;
								resolve_conflict(); 
								cout<<"\n Exiting... ";
								exit(0);
							}
						}
					}
				}
			}	
		}
	}
	// STEP 4-------------------
	for(set<char>::iterator it=derives[start_symbol].begin(); it!=derives[start_symbol].end(); it++)
	{
		pr = make_pair('$',*it);
		if(parsing_table.find(pr)==parsing_table.end())
		{
			parsing_table[pr] = '<';
		}
		else
		{
			cout<<"\n Following conflict occurred while constructing table : "<<endl;
			cout<<" for ("<<"$"<<","<<*it<<") '<' as well as '"<<parsing_table[pr]<<"'"<<endl;
			resolve_conflict(); 
			cout<<"\n Exiting... ";
			exit(0);
		}	
	}
	for(set<char>::iterator it=derives_back[start_symbol].begin(); it!=derives_back[start_symbol].end(); it++)
	{
		pr = make_pair(*it,'$');
		if(parsing_table.find(pr)==parsing_table.end())
		{
			parsing_table[pr] = '>';
		}
		else
		{
			cout<<"\n Following conflict occurred while constructing table : "<<endl;
			cout<<" for ("<<*it<<","<<"$"<<") '<' as well as '"<<parsing_table[pr]<<"'"<<endl;
			resolve_conflict(); 
			cout<<"\n Exiting... ";
			exit(0);
		}	
	}
	
}

void table_print()
{
	cout<<" Table Entries Are : "<<endl;
	cout<<" \t";
	vector<char>vec;
	vec.push_back('R'); vec.push_back('S'); vec.push_back('T'); vec.push_back('a');vec.push_back('^'); vec.push_back(','); vec.push_back('('); vec.push_back(')'); vec.push_back('$');
	
	for(vector<char>::iterator i=vec.begin(); i!=vec.end(); i++)
	{
		cout<<*i<<"\t";
	}
	cout<<endl;

	pair<char,char>pr;
	for(vector<char>::iterator i=vec.begin(); i!=vec.end(); i++)
	{
		cout<<*i<<": \t";
		for(vector<char>::iterator j=vec.begin(); j!=vec.end(); j++)
		{
			pr = make_pair(*i, *j);
			if(parsing_table.find(pr)==parsing_table.end())
			{
				cout<<" "<<"\t";
			}
			else
			{
				cout<<parsing_table[pr]<<"\t";
			}
		}
		cout<<endl;
	}
	
	
//	for(set<char>::iterator i=symbols.begin(); i!=symbols.end(); i++)
//	{
//		cout<<*i<<"\t";
//	}
//	cout<<endl;
//
//	pair<char,char>pr;
//	for(set<char>::iterator i=symbols.begin(); i!=symbols.end(); i++)
//	{
//		cout<<*i<<": \t";
//		for(set<char>::iterator j=symbols.begin(); j!=symbols.end(); j++)
//		{
//			pr = make_pair(*i, *j);
//			if(parsing_table.find(pr)==parsing_table.end())
//			{
//				cout<<" "<<"\t";
//			}
//			else
//			{
//				cout<<parsing_table[pr]<<"\t";
//			}
//		}
//		cout<<endl;
//	}
}


char reduce_str(string s)
{
	for(set<char>::iterator it = non_term.begin(); it!= non_term.end(); it++ )
	{
		char nt = *it;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string stt = *j;
			if(stt==s) return nt;
		}
	}
	cout<<" Could not reduce : "<<s<<endl;
	cout<<"\n Not Accepted "<<endl;
	exit(0);
}



void parse_string()
{
	string stk_content;
	string inp_content;
	inp_content = str;
	stk_content = "$";
	cout<<" parsing string ... "<<str<<endl;
	pair<char,char>pr;
	stack<char>stk;
	stk.push('$');
	stack<char>top_most_terminal;
	top_most_terminal.push('$');
	int flg=1;
	for(int i=0;i<str.size();i++)
	{
		cout<<stk_content<<"\t\t"<<inp_content<<endl;
		// cout<<"\n begining loop iteration : "<<stk.top()<<" "<<top_most_terminal.top()<<" "<<str[i]<<endl;
		if(stk.top()=='$'&&str[i]=='$')
		{
			cout<<"\n Not Accepted "<<endl;
			return;
		}
		else if(stk.top()==start_symbol && stk.size()==2 && str[i]=='$' )
		{
			cout<<"\n Accepted "<<endl;
			return;
		}
		pr = make_pair(top_most_terminal.top(),str[i]);
		// cout<<"Next pair : ("<<top_most_terminal.top()<<","<<str[i]<<")"<<endl;
		if(parsing_table.find(pr)!=parsing_table.end() && (parsing_table[pr]=='<'||parsing_table[pr]=='=') )
		{
			// cout<<" shift "<<str[i]<<endl;
			top_most_terminal.push(str[i]);
			stk.push(str[i]);
			stk_content+=str[i];
			inp_content = inp_content.substr(1,inp_content.size()-1);
			continue;
		}
		else if(parsing_table.find(pr)!=parsing_table.end() && parsing_table[pr]=='>')
		{
			// cout<<" Reduce ";
			string ss;
			char tp = top_most_terminal.top();
			top_most_terminal.pop();
			ss+=stk.top();
			stk.pop();
			if(stk.empty())
			{
				flg=0;
				break;
			}
			while(stk.top()!=top_most_terminal.top())
			{
				ss+=stk.top();
				stk.pop();
				if(stk.empty())
				{
					flg=0;
					break;
				}
			}
			pr=make_pair(top_most_terminal.top(),tp);
			// cout<<"Next pair : ("<<top_most_terminal.top()<<","<<tp<<")"<<endl;
			while(parsing_table.find(pr)!=parsing_table.end() && (parsing_table[pr]=='='||parsing_table[pr]=='>') )
			{
				// cout<<"\n Now It did Execute.!!"<<endl;
				tp = top_most_terminal.top();
				top_most_terminal.pop();
				while(stk.top()!=top_most_terminal.top())
				{
					ss+=stk.top();
					stk.pop();
					if(stk.empty())
					{
						flg=0;
						break;
					}
				}
				if(flg==0) break;
				pr=make_pair(top_most_terminal.top(),tp);
				// cout<<"Next pair : ("<<top_most_terminal.top()<<","<<tp<<")"<<endl;
			}
			stk_content = stk_content.substr(0,stk_content.size()-ss.size());
			if(flg==0)
			{
				break;
			}
			else
			{
				reverseStr(ss);
				char cr = reduce_str(ss);
				stk.push(cr);
				top_most_terminal.push(cr);
				stk_content+=cr;
			}
			i--;
		}
		else
		{
			cout<<"\n Not Accepted"<<endl;
			return;
		}
	}
	if(flg==0)
	{
		cout<<"\n Not Accepted "<<endl;
		return;
	}
	else if(stk.top()==start_symbol)
	{
		cout<<" Accepted "<<endl;
	}
}




int main()
{
	int n;
	
	char non_ter[10];
	// cout<<"\n Enter the no. of non_terminals in the grammer : "<<endl;
	cin>>n;
	// cout<<"Enter The non_terminals : "<<endl;
	for(int i=0;i<n;i++)
	{
		cin>>non_ter[i];
		non_term.insert(non_ter[i]);
		symbols.insert(non_ter[i]);
	}
		
	start_symbol = non_ter[0];
	int m; char cc;
	// cout<<"\n Enter the no. of terminals in the grammer : "<<endl;
	cin>>m;
	// cout<<"Enter The terminals : "<<endl;
	for(int i=0;i<m;i++)
	{
		cin>>cc;
		term.insert(cc);
		symbols.insert(cc);
	}
	term.insert('$'); // also add $ to the set of terminals
	symbols.insert('$');
	
	string ch;
	// cout<<"\n Enter productions for each non_terminals : \n";
	for(int i=0;i<n;i++)
	{
		// cout<<"\n for ( # to finish )"<<non_ter[i]<<endl;
		prod[non_ter[i]] = st;
		cin>>ch;
		while(ch!="#")
		{
			prod[non_ter[i]].insert(ch);
			cin>>ch;
		}
	}

	first_calc();
	derives_calc();
	derives_backward_calc();
	table_calc();
	table_print();
//	 cout<<"\n Enter String to parse : "<<endl;
	cin>>str;
	cout<<" Input String : "<<str<<endl;
	parse_string();
	// while(1);
	return 0;
}



