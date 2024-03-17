use std::{env::args, fs};

use crate::domain::DomainTag;

pub mod compiler;
pub mod domain;
pub mod fragment;
pub mod message;
pub mod position;
pub mod scanner;
pub mod token;

fn main() {
    let args: Vec<String> = args().collect();

    if args.len() != 2 {
        println!("usage: cargo run <path_to_file>");
        return;
    }

    let file_path = &args[1];

    let tmp = fs::read_to_string(file_path).unwrap();
    let program = Box::leak(tmp.into_boxed_str());

    let compiler = compiler::Compiler::new();

    let mut scanner = scanner::Scanner::new(program, compiler);

    println!("TOKENS:");
    loop {
        let token = scanner.next_token();

        println!("{}", token);

        if let DomainTag::EOP = token.get_domain_tag() {
            break;
        }
    }

    println!("\nMESSAGES");

    let messages = scanner.get_messages();

    for (pos, msg) in messages {
        println!("{}: {}", msg, pos);
    }

    println!("\nIDENTIFIERS");

    let identifiers = scanner.get_names();
    for i in 0..identifiers.len() {
        println!("{}: {}", i, identifiers[i]);
    }
}
