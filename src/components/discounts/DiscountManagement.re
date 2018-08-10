module DiscountManager = ItemManager.Create(DiscountStore);

let component = ReasonReact.statelessComponent("DiscountManagement");

let newItemFromValues =
    (values: DiscountEdit.EditDiscountForm.values)
    : DiscountStore.newItem => {
  name: values.name,
  percent: values.percent |> Percent.toT,
};

let itemFromValues =
    (
      discount: DiscountStore.item,
      values: DiscountEdit.EditDiscountForm.values,
    )
    : DiscountStore.item => {
  id: discount.id,
  name: values.name,
  percent: values.percent |> Percent.toT,
};

let renderColumns: array(DiscountManager.columnRenderer) = [|
  {name: "name", render: discount => ReactUtils.s(discount.name)},
  {
    name: "percent",
    render: discount => ReactUtils.s(discount.percent |> Percent.toDisplay),
  },
|];

let renderCreate = (~items as discounts, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discounts
    onSubmit=(({values}) => onSubmit(newItemFromValues(values)))
    onCancel
  />;

let renderEdit =
    (~items as discounts, ~item as discount, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discount=(Some(discount))
    discounts
    onSubmit=(({values}) => onSubmit(itemFromValues(discount, values)))
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <DiscountManager
      headerKey="admin.discounts.header"
      columnKeyPrefix="discount"
      renderCreate
      renderEdit
      renderColumns
    />,
};