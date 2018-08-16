module CashierManager = ItemManager.Create(CashierStore);

let component = ReasonReact.statelessComponent("CashierManagement");

let mapValuesToNewCashier =
    (values: CashierEdit.EditCashierForm.values)
    : Cashier.New.t => {
  name: values.name,
  pin: values.pin,
};

let mapValuesToCashier =
    (cashier: Cashier.t, values: CashierEdit.EditCashierForm.values)
    : Cashier.t => {
  id: cashier.id,
  name: values.name,
  pin: values.pin,
};

let isUnique = (cashiers, originalPin, pin) =>
  if (originalPin === pin) {
    None;
  } else {
    let duplicates = cashiers |> List.filter((c: Cashier.t) => c.pin === pin);
    let isDuplicate = duplicates |> List.length > 0;
    isDuplicate ? Some("validation.duplicate") : None;
  };

let renderColumns: array(CashierManager.columnRenderer) = [|
  {nameKey: "cashier.name", render: cashier => ReactUtils.s(cashier.name)},
  {nameKey: "cashier.pin", render: cashier => ReactUtils.s(cashier.pin)},
|];

let renderCreate = (~items as cashiers, ~onSubmit, ~onCancel) =>
  <CashierEdit
    isUnique=(isUnique(cashiers))
    onSubmit=(({values}) => onSubmit(mapValuesToNewCashier(values)))
    onCancel
  />;

let renderEdit = (~items as cashiers, ~item as cashier, ~onSubmit, ~onCancel) =>
  <CashierEdit
    cashier
    isUnique=(isUnique(cashiers))
    onSubmit=(({values}) => onSubmit(mapValuesToCashier(cashier, values)))
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <CashierManager
      name="cashier"
      headerKey="admin.cashiers.header"
      renderCreate
      renderEdit
      renderColumns
    />,
};