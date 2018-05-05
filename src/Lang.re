
type message = {
  key: string,
  language: string,
  content: string,
};


let m = (key, language, content) => {key, language, content};

[@bs.module] external en : Js.Array.t('a) = "./lang/en.json";

let enMessages = en |> Js.Array.map(x=> {
  m(x##key, "EN", x##content);
}) |> Array.to_list;

/* 
let menuItems = [
  m("menu.openOrders", "EN", "Open Orders"),
  m("menu.closedOrders", "EN", "Closed Orders"),
  m("menu.expenses", "EN", "Expenses"),
  m("menu.daily", "EN", "Daily"),
  m("menu.admin", "EN", "Admin"),
];

let adminItems = [
  m("admin.logs", "EN", "Open Orders"),
  m("admin.products", "EN", "Products"),
  m("admin.expenseTypes", "EN", "Expense Types"),
  m("admin.vendors", "EN", "Vendors"),
  m("admin.config", "EN", "Configuration"),
];

let productAdmin = [
  m("admin.products.bulkImport.instructions", 
  "EN", 
  "Note: Tab separated values (TSV), one product per line, value order should match the table above.")
];

let nav = [
  m("nav.back", "EN", "Back")
];
let commonActions = [
  m("action.save", "EN", "Save"),
  m("action.create", "EN", "Create"),
  m("action.delete", "EN", "Delete"),
  m("action.pay", "EN", "Pay"),
  m("action.return", "EN", "Return"),
]; */

let dictionary: list(message) =
  List.concat([
    enMessages
  ]);

let translate = key => {
  let language = Config.App.get().language;
  let matches =
    dictionary
    |> List.filter((message: message) =>
         message.key === key && message.language === language
       );
  switch (matches |> List.length) {
  | 0 => key ++ "." ++ language
  | _ => (matches |. List.nth(0)).content
  };
};