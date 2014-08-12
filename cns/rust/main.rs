#![feature(macro_rules)]

extern crate regex;
extern crate getopts;
use std::os;

use sim::Sim;
use args::parse_args;
use constants::parse_constants;

mod layers {
    pub mod layer;
}
mod sim;
mod args;
mod constants;



fn main() {
    let a = os::args();
    let args = try!(parse_args(a));

    println!("{}", args.to_string());
    
    let c = try!(parse_constants(args.constantFilename));

    println!("{}", c.to_string());

    let s = Sim::new(&c);
    println!("{}", s);
}
