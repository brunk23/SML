#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "../shared/sml_shared.h"

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::ifstream;
using std::setfill;

#define CODE 0x1
#define DATA 0x2
#define STRI 0x4

void define_codes(const char *[]);

int main(int argc, char *argv[])
{
  const char *opcodemap[MAXOP];
  int dtype[MEMSIZE], contents[MEMSIZE];
  char vname[MEMSIZE][4], cname[4] = { 'a', 'a', 'a', 0 };
  int returnCode = 0, counter = 0, input = 0;
  int scount = 0, top = 0, bottom = 0;
  bool indirect = false;
  
  for(int x = 0; x < MEMSIZE; ++x) {
    dtype[x] = 0;
    contents[x] = 0;
    for( int y = 0; y < 4; ++y) {
      vname[x][y] = 0;
    }
  }
  dtype[0] |= CODE;

  /*
   * This is not done. It should recognize labels, strings, data
   * and pop+inst instructions and print them all correctly
   */
  
  define_codes(opcodemap);

  if( argc > 1 ) {
    ifstream filename;
    filename.open(argv[1], std::ios::in);
    if( !filename.is_open() ) {
      cout << "Unable to open file" << endl;
      return 1;
    }
    while( !filename.eof() ) {
      filename >> input;
      if( filename.fail() ) {
	filename.clear();
	filename.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	continue;
      }
      contents[counter] = input;
      top = contents[counter] / OPFACT;
      bottom = contents[counter] % OPFACT;

      if( dtype[counter] & CODE ) {
	if( vname[bottom][0] == 0 ) {
	  bool inc = true;
	  for( int y = 3; y >= 0; --y ) {
	    vname[bottom][y] = cname[y];
	    if( inc && cname[y] > 0 ) {
	      if( cname[y] < 'z' ) {
		cname[y]++;
		inc = false;
	      } else {
		cname[y] = 'a';
	      }
	    }
	  }
	}
	if( top != HALT && top != RET && top != BRANCH) {
	  dtype[counter+1] |= CODE;
	}
	if( top == SREAD || top == SWRITE ) {
	  dtype[bottom] |= STRI;
	} else {
	  if( top == BRANCH || top == BRANCHNEG ||
	      top == BRANCHZERO || top == CALL ) {
	    dtype[bottom] |= CODE;
	  } else {
	    dtype[bottom] |= DATA;
	  }
	}
      }	
      counter++;
    }
    cout << right << setw(4) << "LOC:" << setw(8)
	 << "VALUE" << setw(6) << "LBL" << setw(13)
	 << "INSTRUCTION" << setw(10) << "ARGUMENT" << endl;
    for( int x = 0; x < MEMSIZE; ++x ) {
      if( dtype[x] ) {
	top = contents[x] / OPFACT;
	bottom = contents[x] % OPFACT;
	cout << right << setfill('0') << setw(3) << x << ":  ";
	cout << setw(7) << contents[x] << "  ";
	cout << left << setfill(' ') << setw(5);
	if( vname[x][0] ) {
	  cout << vname[x];
	} else {
	  cout << "   ";
	}
	if( dtype[x] & CODE ) {
	  if( top > MAXOP ) {
	    indirect = true;
	    top -= MAXOP;
	  }
	  if( top < MAXOP)  {
	    cout << setw(8) << opcodemap[top];
	    if( top == POP || top == PUSH ) {
	      cout << setw(1) << "+" << setw(5) << opcodemap[bottom];
	    } else {
	      cout << setw(10) << right;
	      if( vname[bottom][0] ) {
		cout << vname[bottom];
	      } else {
		cout << bottom;
	      }
	    }
	    if( indirect ) {
	      cout << " ** indirect **";
	    }
	  }
	} else {
	  if( dtype[x] & DATA ) {
	    cout << "#" << setw(6) << contents[x];
	  }

	  
	  if( dtype[x] & STRI ) {
	    scount = top/2 + 1;
	    cout << "   \"" << static_cast<char>(bottom);
	    while (scount > 0 ) {
	      --scount;
	      ++x;
	      cout << static_cast<char>(contents[x] / OPFACT);
	      if ( contents[x] > 0 ) {
		cout << static_cast<char>(contents[x] % OPFACT);
	      }
	    }
	    --x;
	    cout << "\"";
	  }
	}
	cout << endl;
	indirect = false;
	counter++;
      }
    }
  } else {
    cout << "You must enter a core file name" << endl;
  }
  return returnCode;
}

void define_codes(const char *opcodemap[]) {
  for( int x = 0; x < MAXOP; ++x ) {
    opcodemap[x] = "INV";
  }
 // These are the currently supported instructions
	
  // Arithmatic
  opcodemap[ADD]="ADD";
  opcodemap[SUBTRACT]="SUBTR";
  opcodemap[MULTIPLY]="MULT";
  opcodemap[DIVIDE]="DIVID";
  opcodemap[MOD]="MODULUS";

  // memory access
  opcodemap[LOAD]="LOAD";
  opcodemap[STORE]="STORE";

  // i/o
  opcodemap[READ]="READ";
  opcodemap[WRITE]="WRITE";
  opcodemap[SREAD]="READSTR";
  opcodemap[SWRITE]="WRITESTR";

  // flow control
  opcodemap[BRANCH]="BRNCH";
  opcodemap[BRANCHNEG]="BRNEG";
  opcodemap[BRANCHZERO]="BRZERO";
  opcodemap[HALT]="HALT";

  // Extended opcodes
  opcodemap[INC]="INCR";
  opcodemap[DEC]="DECR";
  opcodemap[DUMP]="DUMP";
  opcodemap[NOP]="NOP";

  // Stack opcodes
  opcodemap[PUSH]="PUSH";
  opcodemap[POP]="POP";
  opcodemap[CALL]="CALL";
  opcodemap[RET]="RETURN";
}
