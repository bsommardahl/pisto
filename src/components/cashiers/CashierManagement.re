module CashierManager = ItemManager.Create(CashierStore);

let component = ReasonReact.statelessComponent("CashierManagement");

let newItemFromValues =
    (values: CashierEdit.EditCashierForm.values)
    : CashierStore.newItem => {
  name: values.name,
  pin: values.pin,
};

let itemFromValues =
    (cashier: CashierStore.item, values: CashierEdit.EditCashierForm.values)
    : CashierStore.item => {
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

let renderItem =
    (~item as cashier: CashierStore.item, ~onEditClick, ~onDeleteClick) =>
  <tr key=cashier.id>
    <td> <Button local=true onClick=onEditClick label="action.edit" /> </td>
    <td> (ReactUtils.s(cashier.name)) </td>
    <td> (ReactUtils.s(cashier.pin)) </td>
    <td>
      <Button
        local=true
        onClick=onDeleteClick
        className="danger-card"
        label="action.delete"
      />
    </td>
  </tr>;

let renderCreate = (~items as cashiers, ~onSubmit, ~onCancel) =>
  <CashierEdit
    isUnique=(isUnique(cashiers))
    onSubmit=(({values}) => onSubmit(newItemFromValues(values)))
    onCancel
  />;

let renderEdit = (~items as cashiers, ~item as cashier, ~onSubmit, ~onCancel) =>
  <CashierEdit
    cashier
    isUnique=(isUnique(cashiers))
    onSubmit=(({values}) => onSubmit(itemFromValues(cashier, values)))
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <CashierManager
      header="admin.cashiers.header"
      name="cashier"
      tableHeaders=[|"name", "pin"|]
      renderCreate
      renderEdit
      renderItem
    />,
};