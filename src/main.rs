mod colors;
use crate::colors::Color;
use crate::colors::BoolColor;
use std::collections::HashMap;


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


fn check_parenthesis(expr: &String) -> bool {
    let mut count: isize = 0;
    
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
fn findVariables(expr: &String) -> [bool; variablesCount as usize] {
    let mut result: [bool; variablesCount as usize] = [false; variablesCount as usize];

    for ch in (*expr).bytes() {
        if 97 <= ch && ch <= 122 {
            let val = ch - ('a' as u8); 
            result[val as usize] = true;
        }
    }

    return result;
}


fn evaluateInduction(node: Node, map: &HashMap<char, bool>) -> Option<bool> {
    if isVariable(node.value) {
        match (*map).get(&node.value) {
            Some(b) => return Some(*b),
            None => return None
        }
    } else {
        if node.value == '!' {
            let right: Node;
            match node.right {
                Some(n) => right = *n,
                None => return None
            }
            match evaluateInduction(right, map) {
                Some(b) => return Some(!b),
                None => return None
            }
        }

        let left: Node;
        let right: Node;

        match node.left {
            Some(n) => left = *n,
            None => return None
        };
        match node.right {
            Some(n) => right = *n,
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

fn main() {
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

    match evaluateInduction(tree, &testMap) {
        Some(b) => println!("Result is {}", b.colorate()),
        None => println!("Fail")
    }
}
