
#include <Classlib/PQueue.h>
#include <iostream.h>
#include <stdlib.h>
#include <Malloc/Allocator.h>

PQueue :: PQueue ( unsigned n )
{
  N = n;
    
  heap = scinew int[ N + 1 ];
  weight = scinew double[ N + 1];
  pos = scinew int [N  + 1 ];

  for ( int i = 0; i <= N; i++ )
      weight[i] = -1;
  
  count = 0;
}

PQueue :: ~PQueue ( )
{
  delete [] heap;
  delete [] weight;
  delete [] pos;
}

PQueue ::  PQueue ( const PQueue &pq )
{
  N = pq.N;
  count = pq.count;
  
  heap = scinew int[ N + 1 ];
  weight = scinew double[ N + 1 ];
  pos = scinew int[ N + 1 ];
  
  for ( int i = 0; i <= count; i++ )
      heap[i] = pq.heap[i];
  for ( i = 0; i <= N; i++ )
  {
       weight[i] = pq.weight[i];
       pos[i] = pq.pos[i];
  }
}

PQueue &  PQueue :: operator = ( const PQueue &pq )
{
  N = pq.N;
  count = pq.count;
  
  delete [] heap;
  delete [] weight;
  delete [] pos;
  
  heap = scinew int[ N + 1 ];
  weight = scinew double[ N + 1 ];
  pos = scinew int[ N + 1 ];
  
  for ( int i = 0; i <= count; i++ )
      heap[i] = pq.heap[i];
  for ( i = 0; i <= N; i++ )
  {
      weight[i] = pq.weight[i];
      pos[i] = pq.pos[i];
  }

  return *this;
}


int  PQueue :: isEmpty ( )
{
  return ( count == 0 );
}

void  PQueue :: upheap ( int k )
{
  double  v    = weight[ heap[k] ];
  int     data = heap[k];
  
  heap[0] = 0;
  weight[0] = -2;

  while ( weight[ heap[ k / 2 ] ] >= v )
  {
        heap[k] = heap[ k / 2 ];
        pos[ heap[k] ] = k;
        k = k / 2;
  }
  heap[k] = data;
  pos[ data ] = k;
}  


int  PQueue :: replace ( int d, double w )
{
  if ( weight[ d] != -1 && weight[d] <= w ) {
    //      return 0;
    weight[ d ] = w;
    downheap( pos[d] );
    return 1;
  }
  
  if ( weight[ d ] == -1 )
  {
     weight[ d] = w;
     count ++;
     heap[ count ] = d;
     upheap( count );
  }
  else
  {
     weight[d] = w;
     upheap( pos[d] );
  }

  return 1;
}

void  PQueue :: downheap ( int k )
{
  int j;

  double v = weight[ heap[k] ];
  int    data = heap[k];
  
  while ( k <= count / 2 )
  {
     j = k + k;
     if ( j < count && weight[ heap[j] ] > weight[ heap[ j + 1 ] ] )
        j ++ ;
     if ( v <= weight[ heap[j] ] )
        break;
     heap[k] = heap[j];
     pos[ heap[k] ] = k;
     k = j;
   }
   heap[k] = data;
   pos[data] = k;
}

int  PQueue :: remove ( )
{
  if ( count == 0 )
     return 0;
  
  int data = heap[1];

  heap[1] = heap[count];
  pos[ heap[1] ] = 1;
  count -- ;

  downheap( 1 );

  weight[data] = -1;  // it is off the queue...

  return data;
}

void PQueue::print() 
{
  int size=1;
  int cpos=1;
  while(cpos <= count) {
    int onthis=size;
    while(onthis-- && (cpos <= count)) {
      cerr << " " << weight[ heap[cpos] ] << " ";
      cpos++;
    }
    size  = size << 1;
    cerr << "\n";
  }
  
}

#if 0
// build a heap of 50 elements...
const int num_vals=16;

void main(int argc, char* argv)
{
  double val[num_vals+1];

  PQueue the_queue(num_vals);

  for(int i=1;i<=num_vals;i++) {
    val[i] = drand48()*100; // get a [0,1] value
    if (the_queue.replace(i,val[i])) {
      cerr << "worked...\n";
    } else {
      cerr << "huh?\n";
    }
  }

  // now take random ones and change the values

  the_queue.print();

  for(int k=0;k<200;k++) {
    int pos = drand48()*(num_vals-1)+1;
    double nval;
    if (drand48() > 0.5) { // make it bigger...
      nval = val[pos]+drand48()*20;
    } else { // make it smaller...
      nval = val[pos]*drand48();
    }
    the_queue.replace(pos,nval);

//    cerr << "Changed " << pos << " to " << nval << " from " << val[pos] << "\n";
    val[pos] = nval;
//    the_queue.print();
  }
  
  // try and delete an element
  the_queue.print();

  for (k=0;k<5;k++) {

    int pos = drand48()*(num_vals-1)+1;
    
    cerr << "Deleting " << pos << " -> " << val[pos] << "\n";
    
    the_queue.replace(pos,-0.5);
    if (the_queue.remove() != pos) {
      cerr << "Couldn't pop!\n";
    }
    the_queue.print();
  }

  // now remove them

  int done=1;
  while(done) {
    done = the_queue.remove();
    if (done)
      cerr << done << " -> " << val[done] << " \n";
  }

  

}

#endif
