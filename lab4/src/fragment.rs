use crate::position;

#[derive(Clone)]
pub struct Fragment {
    starting: position::Position,
    following: position::Position,
}

impl Fragment {
    pub fn new(starting: position::Position, following: position::Position) -> Self {
        Self {
            starting,
            following,
        }
    }

    pub fn starting(&self) -> position::Position {
        self.starting.clone()
    }

    pub fn following(&self) -> position::Position {
        self.following.clone()
    }
}
