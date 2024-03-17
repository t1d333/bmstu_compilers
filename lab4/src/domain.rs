use std::fmt::Display;

#[derive(Debug)]
pub enum DomainTag {
    IDENT,
    NUMBER,
    OPERATION,
    EOP,
}

impl Display for DomainTag {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            DomainTag::IDENT => write!(f, "IDENT"),
            DomainTag::NUMBER => write!(f, "NUMBER"),
            DomainTag::OPERATION => write!(f, "OPERATION"),
            DomainTag::EOP => write!(f, "EOP"),
        }
    }
}
