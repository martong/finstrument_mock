#include "FooFixture.hpp"
#include "VirtX.hpp"
#include <cstdio>
#include <cstring>
#include <stddef.h>

void VirtualMemberCaller(VirtX&, int);

VirtX* global_vx;
void fake_foo(VirtX* self, int p) {
    ::ftest::called.insert(address(&fake_foo));
    EXPECT_EQ(self, global_vx);
    EXPECT_EQ(p, 13);
}

TEST_F(FooFixture, VirtualMemberCall) {

    global_vx = new VirtX;

    printf("foo addr: %p\n",
           ::fake::address_of_virtual_fun(global_vx, &VirtX::foo));

    SUBSTITUTE_VIRTUAL(&VirtX::foo, global_vx, &fake_foo);

    VirtualMemberCaller(*global_vx, 13);

    EXPECT_EQ(::ftest::called.count(
                  fake::address_of_virtual_fun(&global_vx, &VirtX::foo)),
              0);
    EXPECT_EQ(::ftest::called.count(fake::address(&fake_foo)), 1);
}


// =============================================================================
/// It is kept here for sake of reference.
/// https://blog.mozilla.org/nfroyd/2014/02/20/finding-addresses-of-virtual-functions/
void* find_foo_address(VirtX* aClass) {
    /* The virtual function table is stored at the beginning of the object.  */
    void** vtable = *(void***)aClass;

    /* This structure is described in the cross-platform "Itanium" C++ ABI:

       http://mentorembedded.github.io/cxx-abi/abi.html

       The particular layout replicated here is described in:

       http://mentorembedded.github.io/cxx-abi/abi.html#member-pointers  */
    struct pointerToMember {
        /* This field has separate representations for non-virtual and virtual
           functions.  For non-virtual functions, this field is simply the
           address of the function.  For our case, virtual functions, this
           field is 1 plus the virtual table offset (in bytes) of the function
           in question.  The least-significant bit therefore discriminates
           between virtual and non-virtual functions.

           "Ah," you say, "what about architectures where function pointers do
           not necessarily have even addresses?"  (ARM, MIPS, and AArch64 are
           the major ones.)  Excellent point.  Please see below.  */
        size_t pointerOrOffset;

        /* This field is only interesting for calling the function; it
           describes the amount that the `this' pointer must be adjusted
           prior to the call.  However, on architectures where function
           pointers do not necessarily have even addresses, this field has the
           representation:

           2 * adjustment + (virtual_function_p ? 1 : 0)  */
        ptrdiff_t thisAdjustment;
    };

    /* Translate from the opaque pointer-to-member type representation to
       the representation given above.  */
    pointerToMember p;
    void((VirtX::*m)(int)) = &VirtX::foo;
    memcpy(&p, &m, sizeof(p));

/* Compute the actual offset into the vtable.  Given the differing meaing
   of the fields between architectures, as described above, and that
   there's no convenient preprocessor macro, we have to do this
   ourselves.  */
#if defined(__arm__) || defined(__mips__) || defined(__aarch64__)
    /* No adjustment required to `pointerOrOffset'.  */
    static const size_t pfnAdjustment = 0;
#else
    /* Strip off the lowest bit of `pointerOrOffset'.  */
    static const size_t pfnAdjustment = 1;
#endif

    size_t offset = (p.pointerOrOffset - pfnAdjustment) / sizeof(void*);

    /* Now grab the address out of the vtable and return it.  */
    return vtable[offset];
}

int printer(VirtX* bp) {
    // BAD
    {
        char* vptr = (char*)bp;
        for (int i = 0; i < 25; ++i) {
            char** vptr2 = (char**)vptr;
            char* entry = *vptr2;
            printf("%p\n", entry);
            ++vptr;
        }
    }
    // BAD
    printf("------\n");
    {
        char** vptr = (char**)bp;
        for (int i = 0; i < 4; ++i) {
            // char** vptr2 = (char**)vptr;
            char* entry = *vptr;
            printf("%p\n", entry);
            ++vptr;
        }
    }
    {
        void** vptr = *((void***)(bp));
        // void** vptr = (void**)bp;
        for (int i = 0; i < 4; ++i) {
            void* entry = *vptr;
            printf("%p\n", entry);
            ++vptr;
        }
    }
    return 0;
}

