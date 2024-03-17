use std::collections::HashMap;

use crate::message::Message;
use crate::position::Position;

pub struct Compiler {
    names: Vec<String>,
    name_codes: HashMap<String, usize>,
    messages: Vec<(Position, Message)>,
}

impl Compiler {
    pub fn new() -> Self {
        Self {
            names: vec![],
            name_codes: HashMap::new(),
            messages: vec![],
        }
    }

    pub fn add_name(&mut self, name: &str) -> usize {
        if let Some(code) = self.name_codes.get(name) {
            return *code;
        }

        let code = self.name_codes.len();
        self.names.push(name.to_string());
        self.name_codes.insert(name.to_string(), code);

        code
    }

    pub fn get_name(&self, code: usize) -> &str {
        &self.names[code]
    }

    pub fn add_message(&mut self, is_err: bool, msg: &str, pos: Position) {
        self.messages.push((pos, Message::new(is_err, msg)));
    }

    pub fn messages(&self) -> &[(Position, Message)] {
        self.messages.as_ref()
    }

    pub fn names(&self) -> &[String] {
        self.names.as_ref()
    }
}

impl Default for Compiler {
    fn default() -> Self {
        Self::new()
    }
}
