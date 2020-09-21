
struc Widget_def
    .TypeId         rb  1
    .vtable         rl  1
end struc

virtual at 0
Widget_def Widget_def
end virtual

struc Widget_t
    .TypeId         rb  1
    .vtable         rl  1
    .Definition     rl  1
    .parent         rl  1
    .HasFocus       rl  1
    .X              rl  1
    .Y              rb  1
    .Width          rl  1
    .Height         rb  1
end struc

struc Widget_vtable
    .Length         rl  1
    .IsDynamic      rb  1
    .GetNextItem    rl  1
    .ctor           rl  1
    .dtor           rl  1
    .MoveTo         rl  1
    .Paint          rl  1
    .Focus          rl  1
    .Unfocus        rl  1
    .SendInput      rl  1
end struc
