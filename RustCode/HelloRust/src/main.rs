use std::io::stdin;
fn main() {
    // println!("Hello, world!");
    let mut msg = String::new();
    println!("please enter message:");
    stdin().read_line(&mut msg).unwrap();
    println!("Message is {}", msg);
}
