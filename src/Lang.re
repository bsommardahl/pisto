type message = {
  key: string,
  language: string,
  content: string,
};

let m = (key, language, content) => {key, language, content};

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

let orderButtons = [
  m("order.save", "EN", "Save"),
  m("order.delete", "EN", "Delete"),
  m("order.pay", "EN", "Pay"),
  m("order.printOrder", "EN", "Print Order"),
  m("order.printReceipt", "EN", "Print Receipt"),
  m("order.defaultCustomerName", "EN", "Beloved Customer"),
];

let dictionary: list(message) =
  List.concat([
    [m("Pisto", "EN", "Pisto")],
    menuItems,
    adminItems,
    orderButtons,
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