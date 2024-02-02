let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd /mnt/datenplatte/Workspace/c/mariebuild
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +1 /mnt/datenplatte/Workspace/c/mariebuild
badd +43 src/build.c
badd +0 src/build.h
badd +0 term:///mnt/datenplatte/Workspace/c/mariebuild//8138:bash
badd +25 src/strlist.h
badd +1 include/mcfg_util.h
badd +0 include/mcfg.h
badd +1 build.mb
badd +4 src/target.h
badd +0 src/target.c
badd +19 build.sh
badd +0 src/logging.h
badd +1 /mnt/datenplatte/Workspace/c/mcfg_2/src
badd +364 /mnt/datenplatte/Workspace/c/mcfg_2/src/mcfg_util.c
badd +0 src/strlist.c
argglobal
%argdel
$argadd /mnt/datenplatte/Workspace/c/mariebuild
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit include/mcfg.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
argglobal
balt include/mcfg_util.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 65 - ((21 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 65
normal! 03|
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
argglobal
if bufexists(fnamemodify("/mnt/datenplatte/Workspace/c/mariebuild/include/mcfg_util.h", ":p")) | buffer /mnt/datenplatte/Workspace/c/mariebuild/include/mcfg_util.h | else | edit /mnt/datenplatte/Workspace/c/mariebuild/include/mcfg_util.h | endif
if &buftype ==# 'terminal'
  silent file /mnt/datenplatte/Workspace/c/mariebuild/include/mcfg_util.h
endif
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 86 - ((21 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 86
normal! 0
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
tabnext
edit /mnt/datenplatte/Workspace/c/mariebuild/build.mb
argglobal
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 49 - ((31 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 49
normal! 024|
lcd /mnt/datenplatte/Workspace/c/mariebuild
tabnext
edit /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 139 - ((43 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 139
normal! 050|
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
argglobal
if bufexists(fnamemodify("/mnt/datenplatte/Workspace/c/mariebuild/src/build.h", ":p")) | buffer /mnt/datenplatte/Workspace/c/mariebuild/src/build.h | else | edit /mnt/datenplatte/Workspace/c/mariebuild/src/build.h | endif
if &buftype ==# 'terminal'
  silent file /mnt/datenplatte/Workspace/c/mariebuild/src/build.h
endif
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
tabnext
edit /mnt/datenplatte/Workspace/c/mariebuild/src/target.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
argglobal
balt /mnt/datenplatte/Workspace/c/mariebuild/src/target.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 80 - ((23 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 80
normal! 019|
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
argglobal
if bufexists(fnamemodify("/mnt/datenplatte/Workspace/c/mariebuild/src/target.h", ":p")) | buffer /mnt/datenplatte/Workspace/c/mariebuild/src/target.h | else | edit /mnt/datenplatte/Workspace/c/mariebuild/src/target.h | endif
if &buftype ==# 'terminal'
  silent file /mnt/datenplatte/Workspace/c/mariebuild/src/target.h
endif
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 16 - ((15 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 0
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
tabnext
edit /mnt/datenplatte/Workspace/c/mariebuild/src/strlist.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
argglobal
balt /mnt/datenplatte/Workspace/c/mariebuild/src/strlist.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 49 - ((32 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 49
normal! 055|
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
argglobal
if bufexists(fnamemodify("/mnt/datenplatte/Workspace/c/mariebuild/src/strlist.h", ":p")) | buffer /mnt/datenplatte/Workspace/c/mariebuild/src/strlist.h | else | edit /mnt/datenplatte/Workspace/c/mariebuild/src/strlist.h | endif
if &buftype ==# 'terminal'
  silent file /mnt/datenplatte/Workspace/c/mariebuild/src/strlist.h
endif
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 25 - ((24 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 25
normal! 059|
lcd /mnt/datenplatte/Workspace/c/mariebuild
wincmd w
exe 'vert 1resize ' . ((&columns * 111 + 111) / 223)
exe 'vert 2resize ' . ((&columns * 111 + 111) / 223)
tabnext
argglobal
if bufexists(fnamemodify("term:///mnt/datenplatte/Workspace/c/mariebuild//8138:bash", ":p")) | buffer term:///mnt/datenplatte/Workspace/c/mariebuild//8138:bash | else | edit term:///mnt/datenplatte/Workspace/c/mariebuild//8138:bash | endif
if &buftype ==# 'terminal'
  silent file term:///mnt/datenplatte/Workspace/c/mariebuild//8138:bash
endif
balt /mnt/datenplatte/Workspace/c/mariebuild/src/build.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 2169 - ((43 * winheight(0) + 22) / 44)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2169
normal! 027|
lcd /mnt/datenplatte/Workspace/c/mariebuild
tabnext 5
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
