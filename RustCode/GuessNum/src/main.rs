use rand::Rng;
use std::{cmp::Ordering, io};

fn main() {
    println!("Guess Num");
    let rand_num = rand::rng().random_range(1..101);
    println!("random num is {}", rand_num);
    loop {
        println!("Please Input Your Guess:");
        let mut guess: String = String::new();
        io::stdin()
            .read_line(&mut guess)
            .expect("Failed to read line");
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Place input a NUM!");
                continue;
            }
        };
        println!("You Guessed {}", guess);

        match guess.cmp(&rand_num) {
            Ordering::Less => println!("Too Small"),
            Ordering::Greater => println!("Too Big"),
            Ordering::Equal => {
                println!("You Win!");
                break;
            }
        }
    }
}
