#[allow(dead_code)]
pub trait Color {
    fn chalk(&self, color: u16) -> String;

    fn red(&self) -> String;
    fn blue(&self) -> String;
    fn green(&self) -> String;
    fn purple(&self) -> String;
    fn yellow(&self) -> String;
    fn cyan(&self) -> String;

    fn light_red(&self) -> String;
    fn light_blue(&self) -> String;
    fn light_green(&self) -> String;
    fn light_purple(&self) -> String;
    fn light_yellow(&self) -> String;
    fn light_cyan(&self) -> String;
}
#[allow(dead_code)]
pub trait BoolColor {
    fn colorate(&self) -> String;
}

impl Color for String {
    fn chalk(&self, color: u16) -> String {
        return format!("\x1b[{}m{}\x1b[0m", color, self);
    }
    fn red(&self) -> String {
        return self.chalk(31);
    }
    fn blue(&self) -> String {
        return self.chalk(34);
    }
    fn green(&self) -> String {
        return self.chalk(32);
    }
    fn purple(&self) -> String {
        return self.chalk(35);
    }
    fn yellow(&self) -> String {
        return self.chalk(33);
    }
    fn cyan(&self) -> String {
        return self.chalk(36);
    }


    fn light_red(&self) -> String {
        return self.chalk(91);
    }
    fn light_blue(&self) -> String {
        return self.chalk(94);
    }
    fn light_green(&self) -> String {
        return self.chalk(92);
    }
    fn light_purple(&self) -> String {
        return self.chalk(95);
    }
    fn light_yellow(&self) -> String {
        return self.chalk(93);
    }
    fn light_cyan(&self) -> String {
        return self.chalk(96);
    }
}
impl BoolColor for bool {
    fn colorate(&self) -> String {
        return format!("\x1b[{}m{}\x1b[0m", if *self {32} else {31}, *self);
    }
}
