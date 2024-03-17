use std::{fmt::Display, iter::Peekable, str::Chars};

#[derive(Clone)]
pub struct Position {
    line: usize,
    pos: usize,
    index: usize,
    iter: Peekable<Chars<'static>>,
}

impl Position {
    pub fn new(iter: Chars<'static>) -> Self {
        Self {
            line: 1,
            pos: 1,
            index: 0,
            iter: iter.peekable(),
        }
    }

    pub fn line(&self) -> usize {
        self.line
    }

    pub fn pos(&self) -> usize {
        self.pos
    }

    pub fn index(&self) -> usize {
        self.index
    }

    pub fn next(&mut self) -> Option<char> {
        if self.is_newline() {
            self.line += 1;
            self.pos = 1;
        } else {
            self.pos += 1;
        }

        self.index += 1;

        if let Some(c) = self.iter.next() {
            return Some(c);
        }

        None
    }

    pub fn cp(&mut self) -> Option<char> {
        if let Some(c) = self.iter.peek() {
            return Some(*c);
        }

        None
    }

    pub fn is_newline(&mut self) -> bool {
        if let Some(c) = self.iter.peek() {
            return c.eq(&'\n');
        }

        false
    }

    pub fn is_whitespace(&mut self) -> bool {
        if let Some(c) = self.iter.peek() {
            return c.is_whitespace();
        }

        false
    }

    pub fn is_alphabetic(&mut self) -> bool {
        if let Some(c) = self.iter.peek() {
            return c.is_alphabetic();
        }

        false
    }

    pub fn is_numeric(&mut self) -> bool {
        if let Some(c) = self.iter.peek() {
            return c.is_numeric();
        }

        false
    }

    pub fn is_alphanumeric(&mut self) -> bool {
        if let Some(c) = self.iter.peek() {
            return c.is_alphanumeric();
        }

        false
    }
}

impl Display for Position {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "({}, {})", self.line, self.pos)
    }
}
