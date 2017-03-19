/*

// Suitable for operator precedence parsing
3 E T F
5 + * ( ) i
E+T T #
T*F F #
(E) i #

// Not Suitable for operator precedence parsing
1 S
2 a b 
aSbS bSaS #


*/


#include<bits/stdc++.h>
using namespace std;

set<char> ct; // always kept empty		
set<string> st;	 // always kept empty		
set<char> term;	// set of terminals	
set<char> non_term;	// set of non_terminals		
map<char,set<string> >prod;	// Productions for each non_terminal
map<char, set<char> > leading;	// leading of each non_terminal
map<char, set<char> > trailing;	// trailing of each non_terminal
map<pair<char,char>, char>parsing_table;   // Parsing table -- indexed on (T, T) --> Relation b/w them( <, >, =) 
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

void leading_calc()
{
	for(set<char>::iterator i = non_term.begin(); i!= non_term.end(); i++ )
	{
		char nt = *i;
		leading[nt] = ct;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			if(s!="#"&&!isupper(s[0]))  // Y is epsilon
			{
				leading[nt].insert(s[0]);
			}
			
			if(s!="#"&&s.size()>=2&&isupper(s[0])&&!isupper(s[1])) // Y is a single non_terminal
			{
				leading[nt].insert(s[1]);
			}

			if(s!="#"&&isupper(s[0]))
			{
				leading[nt].insert(s[0]); // A->B(alpha) ; s[0]<-->B
				// later replace B with its leading-set
			}
		}
	}

	// Add leading of B to A ... As mentioned in the comment above
	int flag=3;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			// if(leading[nt])
			for(set<char>::iterator j=leading[nt].begin(); j!=leading[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					for(set<char>::iterator k=leading[ch].begin(); k!=leading[ch].end(); k++)
					{
						leading[nt].insert(*k);
					}
				}
			}
		}
	}

	// Now remove B from A as Leading and Trailing are set of Terminals
	flag=2;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			// if(leading[nt])
			for(set<char>::iterator j=leading[nt].begin(); j!=leading[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					leading[nt].erase(ch);
					j--;
				}
			}
		}
	}

	//Printing set of leading of each non_terminals
	cout<< "\n Leading(A) "<<endl;
	for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
	{
		char nt=*i;
		cout<<nt<<" --- ";
		// if(leading[nt])
		for(set<char>::iterator j=leading[nt].begin(); j!=leading[nt].end(); j++)
		{
			char ch = *j;
			cout<<ch<<" ";
		}
		cout<<" "<<endl;
	}

}

void trailing_calc()
{
	for(set<char>::iterator i = non_term.begin(); i!= non_term.end(); i++ )
	{
		char nt = *i;
		trailing[nt]=ct;
		for( set<string>::iterator j = prod[nt].begin(); j!= prod[nt].end(); j++)
		{
			string s = *j;
			if(s!="#"&&!isupper(s[s.size()-1]))  // 'del' is epsilon
			{
				trailing[nt].insert(s[s.size()-1]);
			}
			
			if(s!="#"&&s.size()>=2&&isupper(s[s.size()-1])&&!isupper(s[s.size()-2])) // Y is a single non_terminal
			{
				trailing[nt].insert(s[s.size()-2]);
			}

			if(s!="#"&&isupper(s[s.size()-1]))
			{
				trailing[nt].insert(s[s.size()-1]); // A->(alpha)B ; s[s.size()-1]<-->B
				// later replace B with its trailing-set
			}
		}
	}

	// Add trailing of B to A ... As mentioned in the comment above
	int flag=3;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			// if(trailing[nt])
			for(set<char>::iterator j=trailing[nt].begin(); j!=trailing[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					for(set<char>::iterator k=trailing[ch].begin(); k!=trailing[ch].end(); k++)
					{
						trailing[nt].insert(*k);
					}
				}
			}
		}
	}

	// Now remove B from A as Leading and Trailing are set of Terminals
	flag=2;
	while(flag--)
	{
		for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
		{
			char nt=*i;
			// if(trailing[nt])
			for(set<char>::iterator j=trailing[nt].begin(); j!=trailing[nt].end(); j++)
			{
				char ch = *j;
				if(isupper(ch))
				{
					trailing[nt].erase(ch);
					j--;
				}
			}
		}
	}

	//Printing set of trailing of each non_terminals
	cout<<" Trailing(A) "<<endl;
	for(set<char>::iterator i = non_term.begin(); i!=non_term.end(); i++)
	{
		char nt=*i;
		cout<<nt<<" --- ";
		// if(trailing[nt])
		for(set<char>::iterator j=trailing[nt].begin(); j!=trailing[nt].end(); j++)
		{
			char ch = *j;
			cout<<ch<<" ";
		}
		cout<<" "<<endl;
	}
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
				if(!isupper(s[i])&&!isupper(s[i+1]))	// Xi and Xi+1 are terminals
				{
					pr = make_pair(s[i],s[i+1]);
					if(parsing_table.find(pr)==parsing_table.end())
					{
						parsing_table[pr] = '=';
					}
					else
					{
						cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
						cout<<"\n Following conflict occurred while constructing table : "<<endl;
						cout<<" for ("<<s[i]<<","<<s[i+1]<<") '=' as well as '"<<parsing_table[pr]<<"'"<<endl; 
						cout<<"\n Exiting... ";
						exit(0);
					}
				}
				
				if(i<=s.size()-3 && !isupper(s[i]) && !isupper(s[i+2]) && isupper(s[i+1]))	// Xi and Xi+2 are terminals and Xi+1 is a non-terminal
				{
					pr = make_pair(s[i],s[i+2]);
					if(parsing_table.find(pr)==parsing_table.end())
					{
						parsing_table[pr] = '=';
					}
					else
					{
						cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
						cout<<"\n Following conflict occurred while constructing table : "<<endl;
						cout<<" for ("<<s[i]<<","<<s[i+2]<<") '=' as well as '"<<parsing_table[pr]<<"'"<<endl; 
						cout<<"\n Exiting... ";
						exit(0);
					}
				}

				if(!isupper(s[i]) && isupper(s[i+1]))	// Xi is terminal and Xi+1 is non-terminal
				{
					for(set<char>::iterator itr = leading[s[i+1]].begin(); itr!=leading[s[i+1]].end(); itr++ )
					{
						pr = make_pair(s[i],*itr);
						if(parsing_table.find(pr)==parsing_table.end())
						{
							parsing_table[pr] = '<';
						}
						else
						{
							cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
							cout<<"\n Following conflict occurred while constructing table : "<<endl;
							cout<<" for ("<<s[i]<<","<<*itr<<") '<' as well as '"<<parsing_table[pr]<<"'"<<endl; 
							cout<<"\n Exiting... ";
							exit(0);
						}
					}
				}

				if(isupper(s[i]) && !isupper(s[i+1]))	// Xi is non-terminal and Xi+1 is terminal
				{
					for(set<char>::iterator itr = trailing[s[i]].begin(); itr!=trailing[s[i]].end(); itr++ )
					{
						pr = make_pair(*itr, s[i+1]);
						if(parsing_table.find(pr)==parsing_table.end())
						{
							parsing_table[pr] = '>';
						}
						else
						{
							cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
							cout<<"\n Following conflict occurred while constructing table : "<<endl;
							cout<<" for ("<<*itr<<","<<s[i+1]<<") '>' as well as '"<<parsing_table[pr]<<"'"<<endl; 
							cout<<"\n Exiting... ";
							exit(0);
						}
					}
				}
			}

		}
	}
	// 3rd step of the algorithm
	for(set<char>::iterator itr = leading[start_symbol].begin(); itr!=leading[start_symbol].end(); itr++ )
	{
		pr = make_pair('$' , *itr);
		if(parsing_table.find(pr)==parsing_table.end())
		{
			parsing_table[pr] = '<';
		}
		else
		{
			cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
			cout<<"\n Following conflict occurred while constructing table : "<<endl;
			cout<<" for ("<<"$"<<","<<*itr<<") '<' as well as '"<<parsing_table[pr]<<"'"<<endl; 
			cout<<"\n Exiting... ";
			exit(0);
		}
	}

	for(set<char>::iterator itr = trailing[start_symbol].begin(); itr!=trailing[start_symbol].end(); itr++ )
	{
		pr = make_pair(*itr, '$');
		if(parsing_table.find(pr)==parsing_table.end())
		{
			parsing_table[pr] = '>';
		}
		else
		{
			cout<<"\n Grammer is not suitable for operator-precedence parsing "<<endl;
			cout<<"\n Following conflict occurred while constructing table : "<<endl;
			cout<<" for ("<<*itr<<","<<"$"<<") '>' as well as '"<<parsing_table[pr]<<"'"<<endl; 
			cout<<"\n Exiting... ";
			exit(0);
		}
	}

}

void table_print()
{
	cout<<" Table Entries Are : "<<endl;
	cout<<" \t";
	for(set<char>::iterator i=term.begin(); i!=term.end(); i++)
	{
		cout<<*i<<"\t";
	}
	cout<<endl;

	pair<char,char>pr;
	for(set<char>::iterator i=term.begin(); i!=term.end(); i++)
	{
		cout<<*i<<": \t";
		for(set<char>::iterator j=term.begin(); j!=term.end(); j++)
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
}

char reduce_str(string s)
{
	if(s.size()==3 && s[1]=='*') { //cout<<"Reduce : "<<s<<" to 'T'"<<endl;
	return 'E'; }
	if(s.size()==3 && s[1]=='+') { //cout<<"Reduce : "<<s<<" to 'E'"<<endl; 
	return 'E'; }
	if(s.size()==3 && (s[1]=='E'||s[1]=='T'||s[1]=='F')) { //cout<<"Reduce : "<<s<<" to 'F'"<<endl; 
	return 'E'; }
	if(s.size()==1 && s[0]=='i') { //cout<<"Reduce : "<<s<<" to 'F'"<<endl; 
	return 'F'; }
	if(s.size()==1 && s[0]=='T') { //cout<<"Reduce : "<<s<<" to 'E'"<<endl; 
	return 'E'; }
	if(s.size()==1 && s[0]=='F') { //cout<<"Reduce : "<<s<<" to 'T'"<<endl; 
	return 'E'; }
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
		else if(stk.top()==start_symbol && stk.size()==2 && top_most_terminal.top()=='$' && top_most_terminal.size()==1 && str[i]=='$' )
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
	}
	term.insert('$'); // also add $ to the set of terminals

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
	leading_calc();
	trailing_calc();
	table_calc();
	table_print();
	// cout<<"\n Enter String to parse : "<<endl;
	cin>>str;
	cout<<" Input String : "<<str<<endl;
	parse_string();
	// while(1);
	return 0;
}



