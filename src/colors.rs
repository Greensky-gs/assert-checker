
pub trait Color {
    fn chalk(&self, color: u16) -> String;

    fn red(&self) -> String;
    fn blue(&self) -> String;
    fn green(&self) -> String;
    fn purple(&self) -> String;
    fn yellow(&self) -> String;
    fn cyan(&self) -> String;

    fn lightRed(&self) -> String;
    fn lightBlue(&self) -> String;
    fn lightGreen(&self) -> String;
    fn lightPurple(&self) -> String;
    fn lightYellow(&self) -> String;
    fn lightCyan(&self) -> String;
}
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


    fn lightRed(&self) -> String {
        return self.chalk(91);
    }
    fn lightBlue(&self) -> String {
        return self.chalk(94);
    }
    fn lightGreen(&self) -> String {
        return self.chalk(92);
    }
    fn lightPurple(&self) -> String {
        return self.chalk(95);
    }
    fn lightYellow(&self) -> String {
        return self.chalk(93);
    }
    fn lightCyan(&self) -> String {
        return self.chalk(96);
    }
}
impl BoolColor for bool {
    fn colorate(&self) -> String {
        return format!("\x1b[{}m{}\x1b[0m", if *self {32} else {31}, *self);
    }
}
