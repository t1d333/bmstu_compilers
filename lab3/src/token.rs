use std::fmt::{self, Display};

#[derive(Debug)]
pub enum TokenKind {
    Coefficient,
    Operator,
    Substance,
}

impl fmt::Display for TokenKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            TokenKind::Coefficient => write!(f, "COEFFICIENT"),
            TokenKind::Operator => write!(f, "OPERATOR"),
            TokenKind::Substance => write!(f, "SUBSTANCE"),
        }
    }
}

#[derive(Debug)]
pub struct Token {
    val: String,
    kind: TokenKind,
    pos: (usize, usize),
}

impl Token {
    pub fn new(val: &str, kind: TokenKind, pos: (usize, usize)) -> Self {
        Self {
            val: val.to_string(),
            kind,
            pos,
        }
    }

    pub fn val(&self) -> &str {
        self.val.as_ref()
    }

    pub fn kind(&self) -> &TokenKind {
        &self.kind
    }

    pub fn pos(&self) -> (usize, usize) {
        self.pos
    }
}

impl Display for Token {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{}",
            format!(
                "{} ({}, {}): {}",
                self.kind(),
                self.pos().0 + 1,
                self.pos().1 + 1,
                self.val()
            )
        )
    }
}
