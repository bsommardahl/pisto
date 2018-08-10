module ProductManager = ItemManager.Create(ProductStore);

let component = ReasonReact.statelessComponent("ProductManagement");

let newItemFromValues =
    (values: ProductEdit.EditProductForm.values)
    : ProductStore.newItem => {
  sku: values.sku,
  name: values.name,
  suggestedPrice: values.price |> Money.toT,
  taxCalculation:
    values.taxCalculationMethod
    ++ "|"
    ++ values.taxRate
    |> Tax.Calculation.toMethod,
  tags: values.tags |> Tags.toList,
  onHand: 0,
  startDate: None,
  endDate: None,
  department: "",
  unit: "",
  products: [],
  weight: 0,
  location: "",
};

let itemFromValues =
    (product: ProductStore.item, values: ProductEdit.EditProductForm.values)
    : ProductStore.item => {
  id: product.id,
  name: values.name,
  sku: values.sku,
  suggestedPrice: values.price |> Money.toT,
  taxCalculation:
    values.taxCalculationMethod
    ++ "|"
    ++ values.taxRate
    |> Tax.Calculation.toMethod,
  tags: values.tags |> Tags.toList,
  onHand: product.onHand,
  startDate: product.startDate,
  endDate: product.endDate,
  department: product.department,
  weight: product.weight,
  unit: product.unit,
  products: product.products,
  location: product.location,
};

let renderColumns: array(ProductManager.columnRenderer) = [|
  {name: "sku", render: product => ReactUtils.s(product.sku)},
  {name: "name", render: product => ReactUtils.s(product.name)},
  {
    name: "price",
    render: product =>
      ReactUtils.s(product.suggestedPrice |> Money.toDisplay),
  },
  {
    name: "taxCalculationMethod",
    render: product =>
      ReactUtils.s(
        product.taxCalculation |> Tax.Calculation.toDelimitedString,
      ),
  },
  {
    name: "tags",
    render: product => ReactUtils.s(product.tags |> Tags.toCSV),
  },
|];

let renderCreate = (~items as products, ~onSubmit, ~onCancel) =>
  <ProductEdit
    products
    onSubmit=(({values}) => onSubmit(newItemFromValues(values)))
    onCancel
  />;

let renderEdit = (~items as products, ~item as product, ~onSubmit, ~onCancel) =>
  <ProductEdit
    product=(Some(product))
    products
    onSubmit=(({values}) => onSubmit(itemFromValues(product, values)))
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <ProductManager
      headerKey="admin.products.header"
      columnKeyPrefix="product"
      renderCreate
      renderEdit
      renderColumns
    />,
};