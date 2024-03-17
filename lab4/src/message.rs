use std::fmt::Display;

#[derive(Debug)]
pub struct Message {
    is_err: bool,
    message: String,
}

impl Message {
    pub fn new(is_err: bool, message: &str) -> Self {
        Self {
            is_err,
            message: message.to_string(),
        }
    }

    pub fn is_err(&self) -> bool {
        self.is_err
    }

    pub fn message(&self) -> &str {
        self.message.as_ref()
    }
}

impl Display for Message {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.is_err {
            return write!(f, "ERR: {}", self.message);
        }

        write!(f, "MSG: {}", self.message)
    }
}
