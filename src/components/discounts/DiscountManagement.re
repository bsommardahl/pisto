module DiscountManager = ItemManager.Create(DiscountStore);

let component = ReasonReact.statelessComponent("DiscountManagement");

let mapValuesToNewDiscount =
    (values: DiscountEdit.EditDiscountForm.values)
    : DiscountStore.newItem => {
  name: values.name,
  percent: values.percent |> Percent.toT,
};

let mapValuesToDiscount =
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
  {
    nameKey: "discount.name",
    render: discount => ReactUtils.s(discount.name),
  },
  {
    nameKey: "discount.percent",
    render: discount => ReactUtils.s(discount.percent |> Percent.toDisplay),
  },
|];

let renderCreate = (~items as discounts, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discounts
    onSubmit=(({values}) => onSubmit(mapValuesToNewDiscount(values)))
    onCancel
  />;

let renderEdit =
    (~items as discounts, ~item as discount, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discount=(Some(discount))
    discounts
    onSubmit=(
      ({values}) => onSubmit(mapValuesToDiscount(discount, values))
    )
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <DiscountManager
      name="discount"
      headerKey="admin.discounts.header"
      renderCreate
      renderEdit
      renderColumns
    />,
};