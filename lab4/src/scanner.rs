use crate::{
    compiler::Compiler,
    message::Message,
    position::Position,
    token::{self, IdentToken, OperationToken, Token},
};

pub struct Scanner {
    compiler: Compiler,
    cur: Position,
    recovering: bool,
}

impl Scanner {
    pub fn new(program: &'static str, compiler: Compiler) -> Self {
        Self {
            compiler,
            cur: Position::new(program.chars()),
            recovering: false,
        }
    }

    pub fn get_messages(&self) -> &[(Position, Message)] {
        self.compiler.messages()
    }

    pub fn get_names(&self) -> &[String] {
        self.compiler.names()
    }

    pub fn next_token(&mut self) -> Box<dyn Token> {
        while self.cur.cp().is_some() {
            while self.cur.is_whitespace() {
                self.cur.next();
            }

            let mut start = self.cur.clone();
            let mut end = self.cur.clone();

            if start.cp().is_none() {
                return Box::new(token::EOPToken::new(&start));
            }

            let mut attr = "".to_string();

            match start.cp().unwrap() {
                '<' => {
                    end.next();
                    if end.cp().is_none() {
                        self.write_syntax_err(start);
                        self.cur = end.clone();
                        return Box::new(token::EOPToken::new(&end));
                    }

                    let c = end.cp().unwrap();
                    match c {
                        '=' => {
                            end.next();
                            self.cur = end.clone();
                            return Box::new(token::OperationToken::new("<=", &start, &end));
                        }
                        _ => {
                            while end.is_numeric() {
                                attr.push(end.cp().unwrap());
                                end.next();
                            }

                            if !attr.is_empty() && end.cp().unwrap().eq(&'>') {
                                end.next();
                                self.cur = end.clone();

                                return Box::new(token::NumberToken::new(
                                    attr.parse::<i128>().unwrap(),
                                    start,
                                    end,
                                ));
                            }

                            self.write_syntax_err(start);
                            self.skip_err();
                        }
                    }
                }
                '=' => {
                    end.next();
                    let mut result = OperationToken::new("=", &start, &end);

                    if let Some('=') = end.cp() {
                        end.next();
                        result = OperationToken::new("==", &start, &end);
                    }

                    self.cur = end;
                    return Box::new(result);
                }

                _ => {
                    if end.is_alphanumeric() {
                        let mut last = 0;
                        let mut counter = 0;
                        let mut last_iter = None;
                        while end.is_alphanumeric() {
                            counter += 1;
                            attr.push(end.cp().unwrap());
                            if end.is_numeric() {
                                last = counter;
                                last_iter = Some(end.clone());
                            }

                            end.next();
                        }

                        if last > 0 {
                            let ident = attr[..last].to_string();
                            let code = self.compiler.add_name(&ident);

                            let mut last_iter = last_iter.unwrap();
                            last_iter.next();
                            self.cur = last_iter.clone();
                            return Box::new(IdentToken::new(code, start, last_iter));
                        } else {
                            self.write_syntax_err(start);
                            self.skip_err();
                        }
                    } else {
                        self.write_syntax_err(start);
                        self.skip_err();
                    }
                }
            }
        }

        Box::new(token::EOPToken::new(&self.cur))
    }

    fn write_syntax_err(&mut self, pos: Position) {
        if self.recovering {
            return;
        }

        self.compiler.add_message(true, "SYNTAX ERROR", pos);
    }

    fn skip_err(&mut self) {
        if self.recovering {
            self.cur.next();
            return;
        }

        self.recovering = true;

        let token = self.next_token();
        self.cur = token.get_coords().starting();

        self.recovering = false;
    }
}
