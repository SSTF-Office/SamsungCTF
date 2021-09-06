open Images;;
open OImages;;

type file_pointer = Null_FP | FP of in_channel;;

class data_feeder =
	object(self)
		val mutable fp = Null_FP
		val mutable cur = 0
		val mutable idx = 0
		method fopen fn =
			fp <- FP (open_in fn)
		method fread =
			if idx = 0 
			then
				match fp with
				| Null_FP -> idx <- 7; cur <- 0
				| FP f -> 
					idx <- 7;
					try
						cur <- int_of_char (input_char f) 
					with End_of_file -> 
						close_in f; fp <- Null_FP; cur <- 0
			else
				idx <- idx - 1;
			let bit = (cur lsr idx) land 1 in
				bit
	end;;

if Array.length Sys.argv < 4 then
	(Printf.printf "Usage: %s [input file] [output file] [file to embed]\n" Sys.argv.(0); exit 0);;

let df = new data_feeder;;
df#fopen(Sys.argv.(3));;
let file = Sys.argv.(1) in
let imgdata = OImages.load file [] in
let format, _ = Images.file_format file in
let img =
	match OImages.tag imgdata with
	| Rgb24 img -> print_endline "Rgb24"; img
	| Index8 img -> print_endline "Index8"; img#to_rgb24
	| _ -> print_endline "Not supported input file"; assert false
in
	Printf.printf "image width: %d\n" img#width;
	Printf.printf "image height: %d\n" img#height;
	for y = 0 to img#height - 1 do
		for x = 0 to img#width - 1 do
			let color = img#get x y in
			let r = color.r - (color.r land 1) + ((color.g lxor color.b lxor df#fread) land 1) in
			img#set x y {r=r;g=color.g;b=color.b}
		done;
	done;
	img#save Sys.argv.(2) (Some format) [Save_Quality 100];
	print_endline "Save done"
