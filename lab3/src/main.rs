pub mod token;
pub mod tokenizer;
use std::env::args;
use std::fs::File;
use std::io::Read;

use crate::tokenizer::TokenizerErr;

fn main() {
    let args: Vec<String> = args().collect();

    if args.len() != 2 {
        println!("usage: cargo run <path_to_file>");
        return;
    }

    let file_name = &args[1];

    let mut file = File::open(file_name).unwrap();

    let mut raw_text = String::new();
    let _ = file.read_to_string(&mut raw_text);

    let mut tokenizer = tokenizer::Tokenizer::new(&raw_text);

    loop {
        let tmp = tokenizer.next();
        match tmp {
            Ok(token) => println!("{}", token),
            Err(err) => {
                println!("{}", err);

                if let TokenizerErr::EndOfStream() = err {
                    return;
                }
            }
        }
    }
}
