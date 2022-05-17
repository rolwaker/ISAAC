#include "isaac64.hpp"

int main()
{
    rndstate st(0);
    st._update(); // necessary to get the same output as the original.
    
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 256; j++) {
            printf("%.16llx", st.next());
            
            if ((j & 3) == 3)
                printf("\n");
        }
    }
}
