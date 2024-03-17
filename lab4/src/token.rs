use std::fmt::Display;

use crate::{domain::DomainTag, fragment::Fragment, position::Position};

pub trait Token: Display {
    fn get_domain_tag(&self) -> DomainTag;
    fn get_coords(&self) -> Fragment;
}

pub struct IdentToken {
    code: usize,
    coords: Fragment,
}

impl IdentToken {
    pub fn new(code: usize, starting: Position, following: Position) -> Self {
        Self {
            code,
            coords: Fragment::new(starting, following),
        }
    }
}

impl Token for IdentToken {
    fn get_domain_tag(&self) -> DomainTag {
        DomainTag::IDENT
    }

    fn get_coords(&self) -> Fragment {
        self.coords.clone()
    }
}

impl Display for IdentToken {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{} {}-{}: {}",
            DomainTag::IDENT,
            self.get_coords().starting(),
            self.get_coords().following(),
            self.code
        )
    }
}

pub struct OperationToken {
    value: String,
    coords: Fragment,
}

impl OperationToken {
    pub fn new(value: &str, starting: &Position, following: &Position) -> Self {
        Self {
            value: value.to_string(),
            coords: Fragment::new(starting.clone(), following.clone()),
        }
    }
}

impl Token for OperationToken {
    fn get_domain_tag(&self) -> DomainTag {
        DomainTag::OPERATION
    }

    fn get_coords(&self) -> Fragment {
        self.coords.clone()
    }
}

impl Display for OperationToken {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{} {}-{}: {}",
            DomainTag::OPERATION,
            self.get_coords().starting(),
            self.get_coords().following(),
            self.value
        )
    }
}

pub struct NumberToken {
    value: i128,
    coords: Fragment,
}

impl NumberToken {
    pub fn new(value: i128, starting: Position, following: Position) -> Self {
        Self {
            value,
            coords: Fragment::new(starting, following),
        }
    }
}

impl Token for NumberToken {
    fn get_domain_tag(&self) -> DomainTag {
        DomainTag::NUMBER
    }

    fn get_coords(&self) -> Fragment {
        self.coords.clone()
    }
}

impl Display for NumberToken {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{} {}-{}: {}",
            DomainTag::NUMBER,
            self.get_coords().starting(),
            self.get_coords().following(),
            self.value
        )
    }
}

pub struct EOPToken {
    coords: Fragment,
}

impl EOPToken {
    pub fn new(end: &Position) -> Self {
        Self {
            coords: Fragment::new(end.clone(), end.clone()),
        }
    }
}

impl Token for EOPToken {
    fn get_domain_tag(&self) -> DomainTag {
        DomainTag::EOP
    }

    fn get_coords(&self) -> Fragment {
        self.coords.clone()
    }
}

impl Display for EOPToken {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{} {}-{}",
            DomainTag::EOP,
            self.coords.starting(),
            self.coords.following()
        )
    }
}
