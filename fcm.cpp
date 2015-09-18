#include"fcm.h"
#include<iostream>
#include<math.h>
#include<ctime>
#include<stdlib.h>
using namespace std;

fcm::fcm()
{
	iteration_number=50;
	num_clasters=1;
	num_vectors=1;
	len_vectors=1;
	old_num_vect=1;
	old_num_clas=1;
	old_len_vect=1;

	data=new double*[num_vectors];
	for(int i=0;i<num_vectors;i++)
		data[i]=new double[len_vectors];
		//matrix centers
	V=new double*[num_clasters];
		for(int i=0;i<num_clasters;i++)
			V[i]=new double[len_vectors];
		//matrix partition
	U=new double*[num_vectors];
		for(int i=0;i<num_vectors;++i)
			U[i]=new double[num_clasters];
		//matrix distance
	D=new double*[num_vectors];
		for(int i=0;i<num_vectors;i++)
			D[i]=new double[num_clasters];
	
}

fcm::~fcm()
{
	for(int i=0;i<num_vectors;i++)
		delete [] data[i];
	delete [] data;

	for(int i=0;i<old_num_clas;i++)
		delete [] V[i];
	delete [] V;

	for(int i=0;i<old_num_vect;i++)
		delete [] U[i];
	delete [] U; 

	for(int i=0;i<old_num_vect;i++)
		delete [] D[i];
	delete [] D;
}


void fcm::set_clasters_number(int claster)
{
	num_clasters=claster;
}


double fcm::vec_distance(double *v1, double *v2, int len)
{
	double r=0.0;
	for(int i=0;i<len;i++)
		r=r+(v1[i]-v2[i])*(v1[i]-v2[i]);
	return sqrt(r);	

}
void fcm::set_data_matrix(double** dat, const int n, const int l)
{
	if((n != num_vectors) && (l != len_vectors))
	{
	    for (int i=0; i < num_vectors; i++)
		delete [] data[i];
	    delete [] data;

	    num_vectors = n;
	    len_vectors = l;


	    data = new double*[num_vectors];
	    for (int i=0; i < num_vectors; i++) 
		data[i] = new double[len_vectors];
	}

	for(int i=0;i<num_vectors;i++)
		for(int j=0;j<len_vectors;j++)
			data[i][j]=dat[i][j];
}
void fcm::calculate()
{
	//free old depended memory
	for(int i=0;i<old_num_clas;i++)
		delete [] V[i];
	delete [] V;

	for(int i=0;i<old_num_vect;i++)
		delete [] U[i];
	delete [] U; 

	for(int i=0;i<old_num_vect;i++)
		delete [] D[i];
	delete [] D;

	//save new memory alloc
	old_num_clas=num_clasters;
	old_num_vect=num_vectors;
	old_len_vect=len_vectors;

	//create depended memory
	V=new double*[num_clasters];
		for(int i=0;i<num_clasters;i++)
			V[i]=new double[len_vectors];
		//matrix partition
	U=new double*[num_vectors];
		for(int i=0;i<num_vectors;++i)
			U[i]=new double[num_clasters];
		//matrix distance
	D=new double*[num_vectors];
		for(int i=0;i<num_vectors;i++)
			D[i]=new double[num_clasters];
	
	//end create depended memory block
	//algorithm cycle
	int count =0;
	set_ones_matrix(U,num_vectors,num_clasters);
	while(count<iteration_number)
	{
		for(int j=0;j<num_clasters;j++)
		{
			for(int l=0;l<len_vectors;l++)
			{
				double t1=0.0;
				double t2=0.0;
				for(int i=0;i<num_vectors;i++)
				{
					t1=t1+U[i][j]*U[i][j]*data[i][l];
					t2=t2+U[i][j]*U[i][j];
				}
				V[j][l]=t1/t2;
			}
		
			for(int i=0;i<num_vectors;i++)
			{
				for(int j=0;j<num_clasters;j++)
					D[i][j]=vec_distance(data[i],V[j],len_vectors);
			}
			for(int i=0;i<num_clasters;i++)
			{
				double t3=0.0;
				if(D[i][j]=0.0) 
					U[i][j]=1.0;
				else 
					for(int z=0;z<num_clasters;z++)
						t3=t3+D[i][j]*D[i][j]/(D[i][z]*D[i][z]);
U[i][j]=1.0/t3;
			}
		}
	count++;		
	}

	//end algorithm cycle

}

void fcm::set_iteration_number(int s)
{
	iteration_number=s;
}

void fcm::set_ones_matrix(double **matrix, int size_f, int size_s)
{
	srand(time(NULL));
	for(int i=0;i<size_f;i++)
	{
		double r=1.0;
		for(int j=0;j<size_s;j++)
			{
				matrix[i][j]=double(rand()%int(r*1000))/1000.0;
				r=r-matrix[i][j];
			}
	}
}

void fcm::show_data_matrix()const
{
	cout<<"\ninput data matrix"<<endl;
}
void fcm::show_rand_matrix()const
{
	cout<<"\nrandomise matrix"<<endl;
}
void fcm::show_distance_matrix()const
{
	cout<<"\n distance matrix"<<endl;
}
void fcm::show_centr_matrix()const
{
	cout<<"\n matrix with central point"<<endl;
}
