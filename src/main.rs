mod colors;
use crate::colors::{BoolColor,Color};
use std::collections::HashMap;
use std::io::{stdin,stdout,Write};

const variablesCount: u8 = 26;

struct Node {
    value: char,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>
}

impl Node {
    fn new(val: char) -> Node {
        return Node {
            value: val,
            left: None,
            right: None
        }
    }

    fn infix(&self) -> String {
        let mut result = String::from("");
       
        match &self.left {
            Some(n) => result.push_str(&n.infix()),
            None => {}
        }
        result.push(self.value);

        match &self.right {
            Some(n) => result.push_str(&n.infix()),
            None => {}
        };

        return result;
    }

    fn update_left(&mut self, other: Node) {
        self.left = Some(Box::new(other));
    }
    fn update_right(&mut self, other: Node) {
        self.right = Some(Box::new(other));
    }

    fn remove_left(&mut self) {
        self.left = None;
    }
    fn remove_right(&mut self) {
        self.right = None;
    }
}

fn precedance(c: char) -> i8 {
    return match c {
        '!' => 3,
        '&' => 2,
        '|' => 1,
        '>' => 0,
        _ => -1
    };
}

fn isOperator(c: char) -> bool {
    return c == '|' || c == '&' || c == '>' || c == '!';
}
fn isParenthesis(c: char) -> bool {
    return c == '(' || c == ')';
}
fn isVariable(c: char) -> bool {
    return ('a' as usize) <= (c as usize) && (c as usize) <= ('z' as usize);
}
fn error_message(message: String) {
    println!("\x1b[31mERROR :{}\x1b[0m", message);
}



fn parse_primary(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let c: char;
    match (*expr).chars().nth(*pos) {
        Some(t) => c = t,
        None => {error_message("no char found".to_string()); return Err::<Node, String>("error".to_string()) }
    }

    if isVariable(c) {
        (*pos)+=1;
        return Ok(Node::new(c));
    } else if c == '(' {
        (*pos)+=1;
        let node: Node;
        match parse_implication(expr, pos) {
            Ok(n) => node = n,
            Err(e) => return Err::<Node, String>(e)
        }
        
        match (*expr).chars().nth(*pos) {
            Some(_) => {},
            None => { error_message("parse primary, missing (".to_string()); return Err::<Node, String>("error".to_string()) }
        }
        (*pos) +=1;
        return Ok(node);
    } else {
        error_message("imcomplete formula".to_string());

        return Err::<Node, String>("error".to_string());
    }
}
fn parse_not(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let c: char;
    match (*expr).chars().nth(*pos) {
        Some(t) => c = t,
        None => { error_message("Unexpected error".to_string()); return Err::<Node, String>("error".to_string()) }
    }

    if c == '!' {
        (*pos) += 1;
        let operand: Node;
        match parse_not(expr, pos) {
            Ok(n) => operand = n,
            Err(e) => return Err::<Node, String>(e)
        }
        let mut node = Node::new('!');
        node.update_right(operand);

        return Ok(node);
    }
    return parse_primary(expr, pos);
}
fn parse_and(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let mut left: Node;
    match parse_not(expr, pos) {
        Ok(n) => left = n,
        Err(e) => return Err::<Node, String>(e)
    }

    let mut op: char;
    while {
        match (*expr).chars().nth(*pos) {
            Some(t) => op = t,
            None => return Ok(left)
        };
        op == '&'
    } {
        (*pos) += 1;
        let right: Node;
        match parse_not(expr, pos) {
            Ok(n) => right = n,
            Err(e) => return Err(e)
        }
        let mut node = Node::new(op);

        node.update_left(left);
        node.update_right(right);
        left = node;
    }

    return Ok(left);
}
fn parse_or(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let mut left: Node;
    match parse_and(expr, pos) {
        Ok(n) => left = n,
        Err(e) => return Err::<Node, String>(e)
    }
    let mut op: char;

    while {
        match (*expr).chars().nth(*pos) {
            Some(t) => op = t,
            None => return Ok(left)
        };
        op == '|'
    } {
        (*pos) += 1;
        let right: Node;
        match parse_and(expr, pos) {
            Ok(n) => right = n,
            Err(e) => return Err::<Node, String>(e)
        };
        let mut node = Node::new(op);
        node.update_left(left);
        node.update_right(right);
        left = node;
    }

    return Ok(left);
}
fn parse_implication(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let mut left: Node;
    match parse_or(expr, pos) {
        Ok(n) => left = n,
        Err(e) => return Err::<Node, String>(e)
    }
    let mut op: char;

    while {
        match (*expr).chars().nth(*pos) {
            Some(c) => op = c,
            None => return Ok(left)
        };
        op == '>'
    } {
        (*pos) += 1;
        let right: Node;
        match parse_or(expr, pos) {
            Ok(n) => right = n,
            Err(e) => return Err::<Node, String>(e)
        }
        let mut node = Node::new(op);
        node.update_left(left);
        node.update_right(right);
        left = node;
    }

    return Ok(left);
}
fn parse(expr: &String, pos: &mut usize) -> Result<Node, String> {
    let result: Node;
    match parse_implication(expr, pos) {
        Ok(n) => result = n,
        Err(e) => return Err::<Node, String>(e)
    }

    if *pos < (*expr).len() {
        return Err::<Node, String>("Unexpected chars at the end".to_string());
    }
    return Ok(result);
}

fn build(expr: &String) -> Result<Node, String> {
    let mut pos: usize = 0;
    let res: Node;

    match parse(expr, &mut pos) {
        Ok(n) => res = n,
        Err(e) => return Err::<Node, String>(e)
    }

    return Ok(res)
}


fn check_components(expr: &String) -> bool {
    for ch in (*expr).chars() {
        if !isVariable(ch) && !isOperator(ch) && !isParenthesis(ch) {
            return false;
        }
    }
    return true;
}
fn check_parenthesis(expr: &String) -> bool {
    let mut count: usize = 0;

    for ch in (*expr).chars() {
        if ch == '(' {
            count+=1;
        }
        if ch == ')' {
            count -= 1;
        }
    }

    return count == 0;
}
fn find_variables(expr: &String, count: &mut u8) -> [bool; variablesCount as usize] {
    let mut result: [bool; variablesCount as usize] = [false; variablesCount as usize];
    *count = 0;

    for ch in (*expr).bytes() {
        if 97 <= ch && ch <= 122 {
            let val  = ch - ('a' as u8);
            if !result[val as usize] {
                *count += 1;
            }
            result[val as usize] = true;
        }
    }

    return result;
}
fn decimal_to_binary(amount: &u64, max: u8) -> Vec<bool> {
    let mut vector = Vec::new();
    let mut copy = *amount;

    let mut i: u8 = 0;
    while i < max {
        vector.push(copy & 1 == 1);
        copy = copy >> 1;
        i += 1;
    };
    return vector;
}
fn create_hashmap(amount: &u64, variables: [bool; variablesCount as usize], count: &u8) -> HashMap<char, bool> {
    let vector = decimal_to_binary(amount, *count);
    let mut map = HashMap::new();

    let mut i = 0;
    let mut index = 0;

    while i < variablesCount {
        map.insert((97 + i) as char, if !variables[i as usize] {false} else {
            let val = vector[index];
            index+=1;
            val
        });

        i+=1;
    }
    return map;
}
fn display_map(map: HashMap<char, bool>) {
    for (key, value) in map {
        println!("{}: {}", key, value.colorate());
    }
}

fn evaluateInduction(node: &Node, map: &HashMap<char, bool>) -> Option<bool> {
    if isVariable(node.value) {
        match (*map).get(&node.value) {
            Some(b) => return Some(*b),
            None => return None
        }
    } else {
        if node.value == '!' {
            match &node.right {
                Some(n) => return Some(!evaluateInduction(n, map)?),
                None => return None
            }
        }

        let left = match &node.left {
            Some(n) => n,
            None => return None
        };
        let right = match &node.right {
            Some(n) => n,
            None => return None
        };

        let leftOperand: bool;
        let rightOperand: bool;

        match evaluateInduction(left, map) {
            Some(b) => leftOperand = b,
            None => return None
        };
        match evaluateInduction(right, map) {
            Some(b) => rightOperand = b,
            None => return None
        };

        match node.value {
            '&' => return Some(leftOperand && rightOperand),
            '|' => return Some(leftOperand || rightOperand),
            '>' => return Some(!leftOperand || rightOperand),
            _ => return None
        }
    }
}

fn get_expression(reference: &mut String) -> bool {
    match stdin().read_line(reference) {
        Ok(_) => return true,
        Err(_) => return false
    }
}
fn check_expression(expression: &String) -> bool {
    return check_parenthesis(expression) && check_components(expression);
}

fn display_header(vars: [bool; variablesCount as usize]) {
    let mut i = 0;
    let mut c = 0;
    while i < variablesCount {
        if vars[i as usize] {
            print!(" \x1b[93m{}\x1b[0m │", (97 + i) as char);
            c+=1;
        }
        i+=1;
    }
    println!("    {}    ", "result".to_string().lightBlue());

    i = 0;
    while i < c * 4 + (8 + 7) {
        print!("{}", if i % 4 == 3 && i > 0 && i <= c * 4 {"┼"} else {"─"});
        i+=1;
    }
    print!("\n");
}
fn display_result(result: bool, vars: [bool; variablesCount as usize], map: HashMap<char, bool>) -> Option<()> {
    let mut underline = String::from("");

    let mut i = 0;
    while i < variablesCount {
        if vars[i as usize] {
            let val: bool = *(map.get(&((97 + i) as char)))?;
            print!(" \x1b[{}m{}\x1b[0m │", 31 + (val as u8), val as u8);
        }
        i+=1;
    }
    println!("      \x1b[{}m{}\x1b[0m", 31 + (result as u8), result as u8);

    return Some(());
}


fn solve(expr: &String, tree: &Node) {
    let mut vars_count = 0;
    let vars = find_variables(expr, &mut vars_count);

    display_header(vars);

    let mut i: u64 = 0;
    while i < 2_u32.pow(vars_count.into()).into() {
        let map = create_hashmap(&i, vars, &(vars_count).into());
        let result: bool;
        match evaluateInduction(tree, &map) {
            Some(r) => result = r,
            None => {
                println!("{}", "Something went wrong".to_string().red());
                return;
            }
        }

        display_result(result, vars, map);
        i+=1;
    }
}

fn main() {
    print!("Enter the expression: ");
    stdout().flush().unwrap();

    let mut expression = String::from("");
    let success = get_expression(&mut expression);

    if !success {
        println!("{}", "The operation failed".to_string().red());
        return;
    }
    expression = expression.trim_end().to_string();

    if !check_expression(&expression) {
        println!("Your expression {} is not valid", expression.lightRed());
        return;
    }

    let tree: Node;
    match build(&expression) {
        Ok(n) => tree = n,
        Err(_) => {
            return println!("Your expression {} cannot be parsed", expression.lightRed());
        }
    }

    solve(&expression, &tree);
}

fn _main() {
    let expression = String::from("a&(b|c)");

    let tree: Node;
    match build(&expression) {
        Ok(n) => tree = n,
        Err(e) => return error_message(e)
    }

    println!("Parsed expression : {}", tree.infix().lightRed());

    let mut testMap = HashMap::new();

    testMap.insert('a', false);
    testMap.insert('b', true);
    testMap.insert('c', true);

    match evaluateInduction(&tree, &testMap) {
        Some(b) => println!("Result is {}", b.colorate()),
        None => println!("Fail")
    }
}
